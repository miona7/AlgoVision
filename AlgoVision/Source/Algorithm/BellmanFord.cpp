#include "BellmanFord.h"

BellmanFord::BellmanFord(const std::shared_ptr<Graph> g) : Algorithm(g) {
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

    clearSteps();

    bellmanFord(idStartNode);

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

    {
        AlgorithmStep s;
        s.m_type  = StepType::UpdateDistance;
        s.m_node  = start;
        s.m_value = 0;
        addStep(s);
    }

    unsigned v = nodes.size();

    // relaksiraj grane v-1 put
    for(int k = 0; k < v - 1; ++k) {
        bool wasRelaxed = false;
        for(const auto& [_, edge]: edges) {
            unsigned u = edge.startNode();
            unsigned v = edge.endNode();
            int      w = edge.getWeight();
            {
                AlgorithmStep s;
                s.m_type = StepType::VisitNode;
                s.m_node = u;
                addStep(s);
            }
            {
                AlgorithmStep s;
                s.m_type    = StepType::ProcessNode; // koristimo kao "pass k"
                s.m_node    = u;
                s.m_value   = k;
                s.m_message = std::string("relaxation pass");
                addStep(s);
            }
            {
                AlgorithmStep s;
                s.m_type  = StepType::ExamineEdge;
                s.m_from  = u;
                s.m_to    = v;
                s.m_value = w; // opcionalno: težina ivice
                addStep(s);
            }
            if(m_minDistance[u] != std::numeric_limits<int>::max() &&
               m_minDistance[u] + w < m_minDistance[v]) {
                m_minDistance[v] = m_minDistance[u] + w;
                wasRelaxed       = true;
                {
                    AlgorithmStep s;
                    s.m_type  = StepType::RelaxEdge;
                    s.m_from  = u;
                    s.m_to    = v;
                    s.m_value = w;
                    addStep(s);
                }
                {
                    AlgorithmStep s;
                    s.m_type  = StepType::UpdateDistance;
                    s.m_node  = v;
                    s.m_value = m_minDistance[v]; // nova distanca
                    addStep(s);
                }
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

QString BellmanFord::resultString() const {
    QString res = "Distances:\n";
    for(const auto& [node, dist]: m_minDistance) {
        res += QString("Node %1 : %2\n").arg(node).arg(dist);
    }
    return res;
}
