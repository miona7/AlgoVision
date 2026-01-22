#include "FloydWarshall.h"

FloydWarshall::FloydWarshall(const std::shared_ptr<Graph> g) : Algorithm(g) {
}

void FloydWarshall::checkConditions() const {
    if(!m_graph || m_graph->getNodes().empty() || !m_graph->isDirected()) {
        throw std::runtime_error("Graph is not initialized or invalid!");
    }
}

void FloydWarshall::execute(unsigned, unsigned) {
    checkConditions();

    clearSteps();

    floydWarshall();

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

// Belezi promene samo kada dodje do smanjenja distanci jer je O(n^3)
void FloydWarshall::floydWarshall() {
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

    for(const auto& [_, edge]: edges) {
        {
            AlgorithmStep s;
            s.m_type = StepType::ExamineEdge;
            s.m_from = edge.startNode();
            s.m_to   = edge.endNode();
            addStep(s);
        }
        {
            AlgorithmStep s;
            s.m_type    = StepType::UpdateDistance;
            s.m_node    = edge.startNode();
            s.m_from    = edge.startNode();
            s.m_to      = edge.endNode();
            s.m_value   = edge.getWeight();
            s.m_message = std::string("init edge weight");
            addStep(s);
        }
    }

    for(const auto& [k, _]: nodes) {
        {
            AlgorithmStep s;
            s.m_type    = StepType::VisitNode;
            s.m_node    = k; // trenutno posrednik
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
                for(const auto& [j, _]: nodes) {
                    if(m_distances[k][j] != std::numeric_limits<int>::max()) {

                        int throughK = m_distances[i][k] + m_distances[k][j];
                        if(throughK < m_distances[i][j]) {
                            m_distances[i][j] = throughK;

                            AlgorithmStep s;
                            s.m_type  = StepType::UpdateDistance;
                            s.m_node  = k; // cvor koji je omogućio poboljšanje
                            s.m_from  = i; // od i
                            s.m_to    = j; // do j
                            s.m_value = throughK;
                            addStep(s);
                        }
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
