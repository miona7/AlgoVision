#include "Prim.h"

Prim::Prim(const std::shared_ptr<Graph>& g) : Algorithm(g) {
}

void Prim::checkConditions() const {
    if(!m_graph || m_graph->getNodes().empty() || m_graph->isDirected()) {
        throw std::runtime_error("Graph is not initialized or invalid!");
    }

    BFS      bfs(m_graph);
    unsigned start = m_graph->getNodes().begin()->first;
    bfs.execute(start);

    for(const auto& [_, visited]: bfs.getVisited()) {
        if(!visited) {
            throw std::runtime_error("Graph is not connected!");
        }
    }
}

void Prim::execute(unsigned, unsigned) {
    checkConditions();

    std::cout << "Starting Prim." << std::endl;
    prim();
    std::cout << "Prim finished." << std::endl;
}

void Prim::prim() {
    std::map<unsigned, bool> inTree;      // da li je cvor vec u drvetu
    std::map<unsigned, int>  minDistance; // minimalno rastojanje cvora do drveta
    std::map<unsigned, int>  parent;      // za svaki cvor pamtimo iz kog cvora smo dosli do njeg

    auto nodes = m_graph->getNodes();
    for(const auto& [nodeId, _]: nodes) {
        inTree[nodeId]      = false;
        minDistance[nodeId] = std::numeric_limits<int>::max();
        parent[nodeId]      = -1;
    }

    std::priority_queue<std::pair<int, unsigned>, std::vector<std::pair<int, unsigned>>,
                        std::greater<>>
             pq;
    unsigned start = nodes.begin()->first;
    pq.emplace(0, start);
    minDistance[start] = 0;

    auto adjList = m_graph->getAdjacencyList();
    auto edges   = m_graph->getEdges();

    while(!pq.empty()) {
        auto [_, currentNode] = pq.top();
        pq.pop();

        if(!inTree[currentNode]) {
            inTree[currentNode] = true;

            for(const auto& [edgeId, neighbourId]: adjList[currentNode]) {
                if(!inTree[neighbourId]) {
                    auto it = edges.find(edgeId);
                    if(it != edges.end()) {
                        int weight = it->second.getWeight();
                        if(weight < minDistance[neighbourId]) {
                            minDistance[neighbourId] = weight;
                            parent[neighbourId]      = currentNode;
                            pq.emplace(minDistance[neighbourId], neighbourId);
                        }
                    }
                }
            }
        }
    }

    int totalWeight = 0;

    std::cout << "Minimum Spanning Tree:" << std::endl;
    for(const auto& [u, v]: parent) {
        if(v != -1) {
            std::cout << v << " -> " << u << " edge = " << minDistance[u] << std::endl;
            totalWeight += minDistance[u];
        }
    }

    std::cout << "Total weight of MST is " << totalWeight << std::endl;
}
