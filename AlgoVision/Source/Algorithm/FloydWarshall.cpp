#include "FloydWarshall.h"

FloydWarshall::FloydWarshall(const std::shared_ptr<Graph> g) : Algorithm(g) {
}

std::optional<AlgorithmError> FloydWarshall::checkConditions() const {
    if(m_graph == nullptr || m_graph->getNodes().empty()) {
        return AlgorithmError{
            AlgorithmErrorType::GraphNotInitialized,
            "Graph is not initialized or empty."
        };
    }

    if(!m_graph->isDirected()) {
        return AlgorithmError{
            AlgorithmErrorType::GraphTypeInvalid,
            "Graph type is invalid."
        };
    }
}

std::optional<AlgorithmError> FloydWarshall::execute(unsigned, unsigned) {
    if(auto err = checkConditions()) {
        return err;
    }

    clearSteps();

    if(auto err = floydWarshall()) {
        return err;
    }

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
        {
            AlgorithmStep s;
            s.m_type = StepType::VisitNode;
            s.m_node = k; // trenutno posrednik
            addStep(s);
        }
        {
            AlgorithmStep s;
            s.m_type    = StepType::ProcessNode;
            s.m_node    = k; // trenutno posrednik
            s.m_message = std::string("using intermediate k");
            addStep(s);
        }
        for(const auto& [i, _]: nodes) {
            if(m_distances[i][k] != std::numeric_limits<int>::max()) {
                auto* edge = m_graph->getEdge(i, k);
                if(edge != nullptr) {
                    AlgorithmStep s;
                    s.m_type = StepType::ExamineEdge;
                    s.m_from = i;
                    s.m_to   = k;
                    addStep(s);
                }
                for(const auto& [j, _]: nodes) {
                    if(m_distances[k][j] != std::numeric_limits<int>::max()) {
                        auto* edge = m_graph->getEdge(k, j);
                        if(edge != nullptr) {
                            AlgorithmStep s;
                            s.m_type = StepType::ExamineEdge;
                            s.m_from = k;
                            s.m_to   = j;
                            addStep(s);
                        }
                        int throughK = m_distances[i][k] + m_distances[k][j];
                        if(throughK < m_distances[i][j]) {
                            m_distances[i][j] = throughK;
                            {
                                AlgorithmStep s;
                                s.m_type = StepType::UpdateDistance;
                                s.m_node = i;
                                addStep(s);
                            }
                            auto* edge = m_graph->getEdge(i, k);
                            if(edge != nullptr) {
                                AlgorithmStep s;
                                s.m_type = StepType::SelectEdge;
                                s.m_from = i;
                                s.m_to   = k;
                                addStep(s);
                            }
                            edge = m_graph->getEdge(k, j);
                            if(edge != nullptr) {
                                AlgorithmStep s;
                                s.m_type = StepType::SelectEdge;
                                s.m_from = k;
                                s.m_to   = j;
                                addStep(s);
                            }
                            {
                                AlgorithmStep s;
                                s.m_type = StepType::UpdateDistance;
                                s.m_node = j;
                                addStep(s);
                            }
                        }
                    }
                }
            }
        }
    }

    for(const auto& [i, _]: nodes) {
        if(m_distances[i][i] < 0) {
            return AlgorithmError{
                AlgorithmErrorType::NegativeCycle,
                "Graph contains a negative cycle."
            };
        }
    }

    return std::nullopt;
}
