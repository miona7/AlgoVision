#include "Dijkstra.h"

Dijkstra::Dijkstra(std::shared_ptr<Graph> g) : Algorithm(g) {
}

bool Dijkstra::checkConditions() const {
    if(!m_graph || m_graph->getNodes().empty()) {
        return false;
    }

    auto edges = m_graph->getEdges();
    for(const auto& [id, edge] : edges) {
        if(edge.getWeight() < 0) {
            return false; // negativna tezina grane, Dijkstra ne radi
        }
    }

    return true;
}

void Dijkstra::execute(const unsigned idStartNode, const unsigned idEndNode) {
    if(!checkConditions()) {
        throw std::runtime_error("Graph is not initialized or invalid, or has negative weight edges!");
    }
    dijkstra(idStartNode);
}

void Dijkstra::dijkstra(unsigned start) {
    auto nodes = m_graph->getNodes();
    std::map<unsigned, bool> finished;      // da li smo nasli rastojanje do cvora
    std::map<unsigned, int> minDistance;    // minimalna rastojanja za svaki cvor

    for(const auto& [nodeId, _] : nodes) {
        finished[nodeId] = false;
        minDistance[nodeId] = std::numeric_limits<int>::max();
    }

    // min-hip: pair<rastojanje, cvor>
    std::priority_queue<std::pair<int, unsigned>, std::vector<std::pair<int, unsigned>>, std::greater<std::pair<int, unsigned>>> pq;
    pq.emplace(0, start);
    minDistance[start] = 0;

    auto adjList = m_graph->getAdjacencyList();
    auto edges = m_graph->getEdges();

    while(!pq.empty()) {
        auto [currentDistance, currentNode] = pq.top(); pq.pop();

        if(finished[currentNode]) {
            continue;
        }
        finished[currentNode] = true;

        if(adjList.find(currentNode) != adjList.end()) {
            for(const auto& [edgeId, neighbourId] : adjList[currentNode]) {
                auto it = edges.find(edgeId);
                if(it != edges.end()) {
                    int weight = it->second.getWeight();
                    if(currentDistance + weight < minDistance[neighbourId]) {
                        minDistance[neighbourId] = currentDistance + weight;
                        pq.emplace(minDistance[neighbourId], neighbourId);
                    }
                }
            }
        }
    }

    std::cout << "Shortest distances from node " << start << ":" << std::endl;
    for(const auto& [id, dist] : minDistance) {
        std::cout << "Node " << id << ": ";
        if(dist == std::numeric_limits<int>::max()) {
            std::cout << "unreachable" << std::endl;
        } else {
            std::cout << dist << std::endl;
        }
    }
}
