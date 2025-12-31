#include "BellmanFord.h"

BellmanFord::BellmanFord(const std::shared_ptr<Graph>& g) : Algorithm(g) {
}

void BellmanFord::checkConditions(unsigned start) const {
    if(!m_graph || m_graph->getNodes().empty() || !m_graph->isDirected() ||
       !m_graph->isWeighted()) {
        throw std::runtime_error("Graph is not initialized or invalid!");
    }

    auto nodes = m_graph->getNodes();
    if(nodes.find(start) == nodes.end()) {
        throw std::runtime_error("Start node does not exist in graph!");
    }
}

void BellmanFord::execute(unsigned idStartNode, unsigned) {
    checkConditions(idStartNode);

    std::cout << "Starting Bellman Ford." << std::endl;
    bellmanFord(idStartNode);
    std::cout << "Bellman Ford finished." << std::endl;

    std::cout << "Shortest distances from node " << idStartNode << ":" << std::endl;
    for(const auto& [node, dist]: m_minDistance) {
        std::cout << "Node " << node << ": ";
        if(dist == std::numeric_limits<int>::max()) {
            std::cout << "unreachable" << std::endl;
        } else {
            std::cout << dist << std::endl;
        }
    }

    if(m_hasNegativeCycle) {
        std::cout << "Graph contains a negative cycle!" << std::endl;
    }
}

void BellmanFord::bellmanFord(unsigned start) {
    m_minDistance.clear();

    auto nodes = m_graph->getNodes();
    auto edges = m_graph->getEdges();

    for(const auto& [node, _]: nodes) {
        m_minDistance[node] = std::numeric_limits<int>::max();
    }

    m_minDistance[start] = 0;

    unsigned v = nodes.size();

    // relaksiraj grane v-1 put
    for(int k = 0; k < v - 1; ++k) {
        bool wasRelaxed = false;
        for(const auto& [_, edge]: edges) {
            unsigned u = edge.startNode();
            unsigned v = edge.endNode();
            int      w = edge.getWeight();
            if(m_minDistance[u] != std::numeric_limits<int>::max() &&
               m_minDistance[u] + w < m_minDistance[v]) {
                m_minDistance[v] = m_minDistance[u] + w;
                wasRelaxed       = true;
            }
        }
        if(!wasRelaxed) {
            break;
        }
    }

    m_hasNegativeCycle = false;
    for(const auto& [_, edge]: edges) {
        unsigned u = edge.startNode();
        unsigned v = edge.endNode();
        int      w = edge.getWeight();
        if(nodes.find(u) != nodes.end() && nodes.find(v) != nodes.end() &&
           m_minDistance[u] != std::numeric_limits<int>::max() &&
           m_minDistance[u] + w < m_minDistance[v]) {
            m_hasNegativeCycle = true;
            break;
        }
    }
}

bool BellmanFord::hasNegativeCycle() const {
    return m_hasNegativeCycle;
}
