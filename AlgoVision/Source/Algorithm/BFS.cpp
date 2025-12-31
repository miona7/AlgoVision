#include "BFS.h"

BFS::BFS(const std::shared_ptr<Graph>& g) : Algorithm(g) {
}

void BFS::checkConditions(unsigned start) const {
    if(!m_graph || m_graph->getNodes().empty()) {
        throw std::runtime_error("Graph is not initialized or invalid!");
    }

    auto nodes = m_graph->getNodes();
    if(nodes.find(start) == nodes.end()) {
        throw std::runtime_error("Start node does not exist in graph!");
    }
}

void BFS::execute(unsigned idStartNode, unsigned) {
    checkConditions(idStartNode);

    std::cout << "BFS traversal starting from node " << idStartNode << ":" << std::endl;
    bfs(idStartNode);
    std::cout << "BFS finished." << std::endl;
}

void BFS::bfs(unsigned start) {
    m_visited.clear();

    auto nodes = m_graph->getNodes();
    for(const auto& [id, _]: nodes) {
        m_visited[id] = false;
    }

    auto adjList = m_graph->getAdjacencyList();

    m_visited[start] = true;
    std::queue<unsigned> q;
    q.push(start);

    while(!q.empty()) {
        unsigned current = q.front(); q.pop();
        std::cout << "visiting node with id " << current << std::endl;
        if(adjList.find(current) != adjList.end()) {
            for(const auto& [_, neighbourId] : adjList[current]) {
                if(!m_visited[neighbourId]) {
                    m_visited[neighbourId] = true;
                    q.push(neighbourId);
                }
            }
        }
    }
}

const std::map<unsigned, bool>& BFS::getVisited() const {
    return m_visited;
}
