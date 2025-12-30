#include "DFS.h"

DFS::DFS(const std::shared_ptr<Graph>& g) : Algorithm(g) {
}

bool DFS::checkConditions() const {
    // graf postoji i ima bar 1 cvor
    return m_graph && !m_graph->getNodes().empty();
}

void DFS::execute(unsigned idStartNode, unsigned idEndNode) {
    if(!checkConditions()) {
        throw std::runtime_error("Graph is not initialized or invalid!");
    }

    auto nodes = m_graph->getNodes();
    if(nodes.find(idStartNode) == nodes.end()) {
        throw std::runtime_error("Start node does not exist in graph!");
    }

    std::map<unsigned, bool> visited;
    for(const auto& [id, node]: nodes) {
        visited[id] = false;
    }

    std::cout << "DFS traversal starting from node " << idStartNode << ":" << std::endl;
    dfs(idStartNode, visited);
    std::cout << "DFS finished." << std::endl;
}

void DFS::dfs(unsigned nodeId, std::map<unsigned, bool>& visited) {
    if(visited[nodeId]) {
        return;
    }
    visited[nodeId] = true;
    std::cout << "visiting node with id " << nodeId << std::endl;

    auto adjList = m_graph->getAdjacencyList();
    if(adjList.find(nodeId) != adjList.end()) {
        for(const auto& [_, neighbourId]: adjList[nodeId]) {
            if(!visited[neighbourId]) {
                dfs(neighbourId, visited);
            }
        }
    }
    // TODO: ispis kroz spdlog
}
