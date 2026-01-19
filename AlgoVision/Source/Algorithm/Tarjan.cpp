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
    {
        AlgorithmStep s;
        s.m_type    = StepType::Start;
        s.m_message = std::string("Tarjan SCC start");
        addStep(s);
    }

    init();

    std::cout << "Starting Tarjan for strongly connected component." << std::endl;
    int  component = 0;
    auto nodes     = m_graph->getNodes();
    for(const auto& [id, node]: nodes) {
        if(m_components[id] == -1) {
            tarjan(id, component);
        }
    }
    std::cout << "Tarjan finished." << std::endl;

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
        std::cout << "\n";
    }

    {
        AlgorithmStep s;
        s.m_type    = StepType::Finish;
        s.m_message = std::string("Tarjan SCC finish");
        addStep(s);
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
        s.m_type    = StepType::UpdateDistance; // koristimo kao "lowlink update"
        s.m_node    = nodeId;
        s.m_value   = m_lowLink[nodeId];
        s.m_message = std::string("lowlink init");
        addStep(s);
    }
    m_tourOrder.push(nodeId);
    m_onStack[nodeId] = true;
    {
        AlgorithmStep s;
        s.m_type = StepType::PushToStack;
        s.m_node = nodeId;
        addStep(s);
    }

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
                s.m_type = StepType::PopFromStack;
                s.m_node = componentNodeId;
                addStep(s);
            }
            {
                AlgorithmStep s;
                s.m_type  = StepType::AssignComponent;
                s.m_node  = componentNodeId;
                s.m_value = component;
                addStep(s);
            }

            m_components[componentNodeId] = component;
            m_onStack[componentNodeId]    = false;

            if(componentNodeId == nodeId) {
                break;
            }
        }
        component++;
    }

    // TODO: ispis kroz spdlog
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
