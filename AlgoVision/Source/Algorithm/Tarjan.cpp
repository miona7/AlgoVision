#include "Tarjan.h"

Tarjan::Tarjan(const std::shared_ptr<Graph> g) : Algorithm(g) {
}

std::optional<AlgorithmError> Tarjan::checkConditions() const {
    if(m_graph == nullptr || m_graph->getNodes().empty()) {
        return AlgorithmError {AlgorithmErrorType::GraphNotInitialized,
                               "Graph is not initialized or empty."};
    }

    if(!m_graph->isDirected()) {
        return AlgorithmError {AlgorithmErrorType::GraphTypeInvalid, "Graph type is invalid."};
    }

    return std::nullopt;
}

std::optional<AlgorithmError> Tarjan::execute(unsigned, unsigned) {
    if(auto err = checkConditions()) {
        return err;
    }

    clearSteps();

    init();

    int  component = 0;
    auto nodes     = m_graph->getNodes();
    for(const auto& [id, node]: nodes) {
        if(m_components[id] == -1) {
            tarjan(id, component);
        }
    }

    std::cout << "Strongly connected components:" << std::endl;
    std::map<int, std::vector<unsigned>> comps;
    for(const auto& [nodeId, compId]: m_components) {
        comps[compId].emplace_back(nodeId);
    }

    for(const auto& [compId, nodesVec]: comps) {
        std::cout << "Component " << compId << ": ";
        for(auto id: nodesVec) {
            std::cout << id << " ";
        }
        std::cout << std::endl;
    }

    return std::nullopt;
}

void Tarjan::tarjan(unsigned nodeId, int& component) {
    m_incomingNumbering[nodeId] = m_lowLink[nodeId] = m_arrivalTime++;
    {
        AlgorithmStep s;
        s.m_type    = StepType::VisitNode;
        s.m_node    = nodeId;
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
        s.m_type    = StepType::UpdateDistance; // koristimo kao "lowlink update"
        s.m_node    = nodeId;
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
                tarjan(neighbourId, component);
                int oldLow        = m_lowLink[nodeId];
                m_lowLink[nodeId] = std::min(m_lowLink[nodeId], m_lowLink[neighbourId]);
                if(m_lowLink[nodeId] != oldLow) {
                    AlgorithmStep s;
                    s.m_type    = StepType::UpdateDistance; // lowlink update
                    s.m_node    = nodeId;
                    addStep(s);
                }
            } else if(m_onStack[neighbourId]) {
                int oldLow        = m_lowLink[nodeId];
                m_lowLink[nodeId] = std::min(m_lowLink[nodeId], m_incomingNumbering[neighbourId]);
                if(m_lowLink[nodeId] != oldLow) {
                    AlgorithmStep s;
                    s.m_type    = StepType::UpdateDistance; // lowlink update
                    s.m_node    = nodeId;
                    addStep(s);
                }
            }
        }
    }

    // ako je cvor koren komponente
    if(m_incomingNumbering[nodeId] == m_lowLink[nodeId]) {
        while(true) {
            unsigned componentNodeId = m_tourOrder.top();
            m_tourOrder.pop();
            {
                AlgorithmStep s;
                s.m_type  = StepType::AssignComponent;
                s.m_node  = componentNodeId;
                addStep(s);
            }

            m_components[componentNodeId] = component;
            m_onStack[componentNodeId]    = false;

            Node* node = m_graph->getNode(componentNodeId);
            if(node != nullptr) {
                node->setComponentColor(component);
            }

            if(componentNodeId == nodeId) {
                break;
            }
        }
        component++;
    }
}

void Tarjan::init() {
    m_arrivalTime = 0;

    m_incomingNumbering.clear();
    m_lowLink.clear();
    m_onStack.clear();
    m_components.clear();

    while(!m_tourOrder.empty()) {
        m_tourOrder.pop();
    }

    auto nodes = m_graph->getNodes();
    for(const auto& [id, _]: nodes) {
        m_incomingNumbering[id] = -1; // jos nije posecen
        m_lowLink[id]           = -1; // lowlink vrednost nepoznata
        m_onStack[id]           = false;
        m_components[id]        = -1; // komponenta nije dodeljena
    }
}
