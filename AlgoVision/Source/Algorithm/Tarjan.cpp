#include "Tarjan.h"

Tarjan::Tarjan(const std::shared_ptr<Graph>& g) : Algorithm(g), m_arrivalTime(0) {
}

bool Tarjan::checkConditions() const {
    // usmereni graf postoji i ima bar 1 cvor
    return m_graph && !m_graph->getNodes().empty() && m_graph->isDirected();
}

void Tarjan::execute(unsigned, unsigned) {
    if(!checkConditions()) {
        throw std::runtime_error("Graph is not initialized or invalid!");
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
}

void Tarjan::tarjan(unsigned nodeId, int& component) {
    m_incomingNumbering[nodeId] = m_lowLink[nodeId] = m_arrivalTime++;
    m_tourOrder.push(nodeId);
    m_onStack[nodeId] = true;

    auto adjList = m_graph->getAdjacencyList();
    if(adjList.find(nodeId) != adjList.end()) {
        for(const auto& [_, neighbourId] : adjList[nodeId]) {
            if(m_incomingNumbering[neighbourId] == -1) {
                tarjan(neighbourId, component);
                m_lowLink[nodeId] = std::min(m_lowLink[nodeId], m_lowLink[neighbourId]);
            } else if(m_onStack[neighbourId]) {
                m_lowLink[nodeId] = std::min(m_lowLink[nodeId], m_incomingNumbering[neighbourId]);
            }
        }
    }

    // ako je cvor koren komponente
    if(m_incomingNumbering[nodeId] == m_lowLink[nodeId]) {
        while(true) {
            unsigned componentNodeId = m_tourOrder.top();
            m_tourOrder.pop();

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
        m_incomingNumbering[id] = -1;         // jos nije posecen
        m_lowLink[id]           = -1;         // lowlink vrednost nepoznata
        m_onStack[id]           = false;
        m_components[id]        = -1;         // komponenta nije dodeljena
    }
}
