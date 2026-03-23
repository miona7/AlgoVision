#include "FloydWarshall.h"

FloydWarshall::FloydWarshall(const std::shared_ptr<Graph> g) : Algorithm(g) {
}

std::optional<AlgorithmError> FloydWarshall::checkConditions() const {
    if(m_graph == nullptr || m_graph->getNodes().empty()) {
        return AlgorithmError{AlgorithmErrorType::GraphNotInitialized,
                              "Graph is not initialized or empty."};
    }

    if(!m_graph->isDirected() || !m_graph->isWeighted()) {
        return AlgorithmError{AlgorithmErrorType::GraphTypeInvalid, "Graph type is invalid."};
    }

    return std::nullopt;
}

std::optional<AlgorithmError> FloydWarshall::execute(const AlgorithmParams&) {
    if(auto err = checkConditions()) {
        return err;
    }

    clearSteps();

    if(auto err = floydWarshall()) {
        return err;
    }

    return std::nullopt;
}

std::optional<AlgorithmError> FloydWarshall::floydWarshall() {
    auto nodes = m_graph->getNodes();
    auto edges = m_graph->getEdges();

    m_distances.clear();

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
        addStep(AlgorithmStep{StepType::VisitNode, k});
        addStep(AlgorithmStep{StepType::ProcessNode, k});

        for(const auto& [i, _]: nodes) {
            if(m_distances[i][k] != std::numeric_limits<int>::max()) {
                auto* edge = m_graph->getEdge(i, k);
                if(edge != nullptr) {
                    addStep(AlgorithmStep{StepType::ExamineEdge, std::nullopt, i, k});
                }

                for(const auto& [j, _]: nodes) {
                    if(m_distances[k][j] != std::numeric_limits<int>::max()) {
                        auto* edge = m_graph->getEdge(k, j);
                        if(edge != nullptr) {
                            addStep(AlgorithmStep{StepType::ExamineEdge, std::nullopt, k, j});
                        }

                        int throughK = m_distances[i][k] + m_distances[k][j];
                        if(throughK < m_distances[i][j]) {
                            m_distances[i][j] = throughK;
                            addStep(AlgorithmStep{StepType::UpdateDistance, i});

                            auto* edge = m_graph->getEdge(i, k);
                            if(edge != nullptr) {
                                addStep(AlgorithmStep{StepType::SelectEdge, std::nullopt, i, k});
                            }

                            edge = m_graph->getEdge(k, j);
                            if(edge != nullptr) {
                                addStep(AlgorithmStep{StepType::SelectEdge, std::nullopt, k, j});
                            }

                            addStep(AlgorithmStep{StepType::UpdateDistance, j});
                        }
                    }
                }
            }
        }
    }

    for(const auto& [i, _]: nodes) {
        if(m_distances[i][i] < 0) {
            return AlgorithmError{AlgorithmErrorType::GraphHasNegativeCycle,
                                  "Graph contains a negative cycle."};
        }
    }

    return std::nullopt;
}

QString FloydWarshall::getResultString() const {
    QString res = "All-pairs shortest distances:\n";

    for(const auto& [i, row]: m_distances) {
        for(const auto& [j, dist]: row) {
            if(dist == std::numeric_limits<int>::max()) {
                res += QString("(%1 -> %2): unreachable\n").arg(i).arg(j);
            } else {
                res += QString("(%1 -> %2): %3\n").arg(i).arg(j).arg(dist);
            }
        }
    }
    return res;
}
