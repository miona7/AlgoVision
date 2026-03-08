#include "Tarjan.h"

Tarjan::Tarjan(const std::shared_ptr<Graph> g) : Algorithm(g) {
}

std::optional<AlgorithmError> Tarjan::checkConditions() const {
    if(m_graph == nullptr || m_graph->getNodes().empty()) {
        return AlgorithmError{AlgorithmErrorType::GraphNotInitialized,
                              "Graph is not initialized or empty."};
    }

    if(!m_graph->isDirected()) {
        return AlgorithmError{AlgorithmErrorType::GraphTypeInvalid, "Graph type is invalid."};
    }

    return std::nullopt;
}

std::optional<AlgorithmError> Tarjan::execute(unsigned, unsigned) {
    if(auto err = checkConditions()) {
        return err;
    }

    clearSteps();

    init();

    auto nodes = m_graph->getNodes();
    for(const auto& [id, node]: nodes) {
        if(m_components[id] == -1) {
            tarjan(id);
        }
    }

    std::map<int, std::vector<unsigned>> comps;
    for(const auto& [nodeId, compId]: m_components) {
        comps[compId].emplace_back(nodeId);
    }

    return std::nullopt;
}

void Tarjan::tarjan(unsigned nodeId) {
    m_incomingNumbering[nodeId] = m_lowLink[nodeId] = m_arrivalTime++;
    {
        AlgorithmStep s;
        s.m_type = StepType::VisitNode;
        s.m_node = nodeId;
        addStep(s);
    }
    {
        AlgorithmStep s;
        s.m_type = StepType::ProcessNode;
        s.m_node = nodeId;
        addStep(s);
    }
    {
        AlgorithmStep s;
        s.m_type = StepType::UpdateDistance; // using as "lowlink update"
        s.m_node = nodeId;
        addStep(s);
    }
    m_tourOrder.push(nodeId);
    m_onStack[nodeId] = true;

    auto adjList = m_graph->getAdjacencyList();
    if(adjList.find(nodeId) != adjList.end()) {
        for(const auto& [_, neighbourId]: adjList[nodeId]) {
            {
                AlgorithmStep s;
                s.m_type = StepType::ExamineEdge;
                s.m_from = nodeId;
                s.m_to   = neighbourId;
                addStep(s);
            }
            if(m_incomingNumbering[neighbourId] == -1) {
                tarjan(neighbourId);
                int oldLow        = m_lowLink[nodeId];
                m_lowLink[nodeId] = std::min(m_lowLink[nodeId], m_lowLink[neighbourId]);
                if(m_lowLink[nodeId] != oldLow) {
                    AlgorithmStep s;
                    s.m_type = StepType::UpdateDistance; // lowlink update
                    s.m_node = nodeId;
                    addStep(s);
                }
            } else if(m_onStack[neighbourId]) {
                int oldLow        = m_lowLink[nodeId];
                m_lowLink[nodeId] = std::min(m_lowLink[nodeId], m_incomingNumbering[neighbourId]);
                if(m_lowLink[nodeId] != oldLow) {
                    AlgorithmStep s;
                    s.m_type = StepType::UpdateDistance; // lowlink update
                    s.m_node = nodeId;
                    addStep(s);
                }
            }
        }
    }

    // node is root of component
    if(m_incomingNumbering[nodeId] == m_lowLink[nodeId]) {
        while(true) {
            unsigned componentNodeId = m_tourOrder.top();
            m_tourOrder.pop();
            {
                AlgorithmStep s;
                s.m_type = StepType::AssignComponent;
                s.m_node = componentNodeId;
                addStep(s);
            }

            m_components[componentNodeId] = m_numComponents;
            m_onStack[componentNodeId]    = false;

            Node* node = m_graph->getNode(componentNodeId);
            if(node != nullptr) {
                node->setComponentColor(m_numComponents);
            }

            if(componentNodeId == nodeId) {
                break;
            }
        }
        m_numComponents++;
    }
}

void Tarjan::init() {
    m_arrivalTime   = 0;
    m_numComponents = 0;

    m_incomingNumbering.clear();
    m_lowLink.clear();
    m_onStack.clear();
    m_components.clear();

    while(!m_tourOrder.empty()) {
        m_tourOrder.pop();
    }

    auto nodes = m_graph->getNodes();
    for(const auto& [id, _]: nodes) {
        m_incomingNumbering[id] = -1; // still not visited
        m_lowLink[id]           = -1; // lowlink unknown
        m_onStack[id]           = false;
        m_components[id]        = -1; // component not assigned
    }
}

QString Tarjan::resultString() const {
    return QString("Number of SCC: %1").arg(m_numComponents);
}
