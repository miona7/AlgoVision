#include "DFS.h"

bool DFS::checkConditions() const {
    if(m_graph == nullptr) {
        return false;
    }

    if(m_graph->size() == 0) {
        return false;
    }

    for(const auto& node: m_graph->getNodes()) {
        if(node == nullptr) {
            return false;
        }
    }

    return true;
}

void DFS::execute(Node* startNode, Node* endNode) {
    if(!checkConditions()) {
        throw std::runtime_error("Graph is not initialized or invalid!");
    }

    int start = 0;
    if(startNode != nullptr) {
        start = startNode->getId();
    } else {
        startNode = m_graph->getNodes()[0];
    }
    std::vector<bool> visited(m_graph->size(), false);
    dfs(start, startNode, visited);
}

void DFS::dfs(int nodeId, Node* node, std::vector<bool>& visited) {
    visited[nodeId] = true;
    for(auto* neighbour: node->getNeighbours()) {
        int id = neighbour->getId();
        if(!visited[id]) {
            dfs(id, neighbour, visited);
        }
    }
    // TODO: ispis kroz spdlog
}
