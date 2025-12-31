#include "FloydWarshall.h"

FloydWarshall::FloydWarshall(const std::shared_ptr<Graph>& g) : Algorithm(g) {
}

void FloydWarshall::checkConditions() const {
    if(!m_graph || m_graph->getNodes().empty() || !m_graph->isDirected()) {
        throw std::runtime_error("Graph is not initialized or invalid!");
    }
}

void FloydWarshall::execute(unsigned, unsigned) {
    checkConditions();

    std::cout << "Starting Floyd Warshall." << std::endl;
    floydWarshall();
    std::cout << "Floyd Warshall finished." << std::endl;

    std::cout << "All-pairs shortest distances:" << std::endl;
    for(const auto& [u, row]: m_distances) {
        for(const auto& [v, dist]: row) {
            std::cout << "From " << u << " to " << v << ": ";
            if(dist == std::numeric_limits<int>::max()) {
                std::cout << "unreachable" << std::endl;
            } else {
                std::cout << dist << std::endl;
            }
        }
    }
}

void FloydWarshall::floydWarshall() {
    auto nodes = m_graph->getNodes();
    auto edges = m_graph->getEdges();

    for(const auto& [u, _]: nodes) {
        for(const auto& [v, _]: nodes) {
            if(u == v) {
                m_distances[u][v] = 0;
            } else {
                m_distances[u][v] = std::numeric_limits<int>::max();
            }
        }
    }

    for(const auto& [_, edge]: edges) {
        m_distances[edge.startNode()][edge.endNode()] = edge.getWeight();
    }

    for(const auto& [k, _]: nodes) {
        for(const auto& [i, _]: nodes) {
            if(m_distances[i][k] != std::numeric_limits<int>::max()) {
                for(const auto& [j, _]: nodes) {
                    if(m_distances[k][j] != std::numeric_limits<int>::max()) {
                        m_distances[i][j] =
                            std::min(m_distances[i][j], m_distances[i][k] + m_distances[k][j]);
                    }
                }
            }
        }
    }

    for(const auto& [i, _]: nodes) {
        if(m_distances[i][i] < 0) {
            throw std::runtime_error("Graph contains a negative cycle!");
        }
    }
}
