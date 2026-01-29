#include "Tarjan.h"

Tarjan::Tarjan(const std::shared_ptr<Graph> g) : Algorithm(g) {
}

void Tarjan::checkConditions() const {
    // usmereni graf postoji i ima bar 1 cvor
    if(!m_graph || m_graph->getNodes().empty() || !m_graph->isDirected()) {
        throw std::runtime_error("Graph is not initialized or invalid!");
    }
}

void Tarjan::execute(unsigned, unsigned) {
    checkConditions();

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
}

void Tarjan::tarjan(unsigned nodeId, int& component) {
    m_incomingNumbering[nodeId] = m_lowLink[nodeId] = m_arrivalTime++;
    {
        AlgorithmStep s;
        s.m_type    = StepType::VisitNode;
        s.m_node    = nodeId;
        s.m_value   = m_incomingNumbering[nodeId]; // index
        s.m_message = std::string("index assigned");
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
        s.m_value   = m_lowLink[nodeId];
        s.m_message = std::string("lowlink init");
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
                    s.m_value   = m_lowLink[nodeId];
                    s.m_message = std::string("lowlink <- min(lowlink, child lowlink)");
                    addStep(s);
                }
            } else if(m_onStack[neighbourId]) {
                int oldLow        = m_lowLink[nodeId];
                m_lowLink[nodeId] = std::min(m_lowLink[nodeId], m_incomingNumbering[neighbourId]);
                if(m_lowLink[nodeId] != oldLow) {
                    AlgorithmStep s;
                    s.m_type    = StepType::UpdateDistance; // lowlink update
                    s.m_node    = nodeId;
                    s.m_value   = m_lowLink[nodeId];
                    s.m_message = std::string("lowlink <- min(lowlink, back-edge index)");
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
                s.m_value = component;
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
