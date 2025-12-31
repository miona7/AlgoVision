#include "BFS.h"

BFS::BFS(const std::shared_ptr<Graph>& g) : Algorithm(g) {
}

bool BFS::checkConditions() const {
    return m_graph && !m_graph->getNodes().empty();
}

void BFS::execute(unsigned idStartNode, unsigned idEndNode) {
    if(!checkConditions()) {
        throw std::runtime_error("Graph is not initialized or invalid!");
    }

    auto nodes = m_graph->getNodes();
    if(nodes.find(idStartNode) == nodes.end()) {
        throw std::runtime_error("Start node does not exist in graph!");
    }

    std::cout << "BFS traversal starting from node " << idStartNode << ":" << std::endl;
    bfs(idStartNode);
    std::cout << "BFS finished." << std::endl;
}

void BFS::bfs(unsigned start) {
    auto nodes = m_graph->getNodes();
    std::map<unsigned, bool> visited;
    for(const auto& [id, _]: nodes) {
        visited[id] = false;
    }

    auto adjList = m_graph->getAdjacencyList();

    visited[start] = true;
    std::queue<unsigned> q;
    q.push(start);

    while(!q.empty()) {
        unsigned current = q.front(); q.pop();
        std::cout << "visiting node with id " << current << std::endl;
        if(adjList.find(current) != adjList.end()) {
            for(const auto& [_, neighbourId] : adjList[current]) {
                if(!visited[neighbourId]) {
                    visited[neighbourId] = true;
                    q.push(neighbourId);
                }
            }
        }
    }
}
