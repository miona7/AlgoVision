#include "DFS.h"

#include "Node.h"

bool DFS::checkConditions() const {
    return true; // TODO
}

void DFS::execute(Node* startNode = nullptr, Node* endNode = nullptr) {
    int start;
    if(startNode) {
        start = startNode->getId();
    } else {
        start     = 0;
        startNode = m_graph->getNodes()[0];
    }
    std::vector<bool> visited(m_graph->size(), false);
    dfs(start, startNode, visited);
}

void DFS::dfs(int nodeId, Node* node, std::vector<bool>& visited) {
    visited[nodeId] = true;
    for(auto neighbour: node->getNeighbours()) {
        int id = neighbour->getId();
        if(!visited[id]) {
            dfs(id, neighbour, visited);
        }
    }
}
