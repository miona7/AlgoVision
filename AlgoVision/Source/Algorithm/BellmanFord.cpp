#include "BellmanFord.h"

BellmanFord::BellmanFord(const std::shared_ptr<Graph> g) : Algorithm(g) {
}

std::optional<AlgorithmError> BellmanFord::checkConditions(unsigned start) const {
    if(m_graph == nullptr || m_graph->getNodes().empty()) {
        return AlgorithmError {AlgorithmErrorType::GraphNotInitialized,
                               "Graph is not initialized or empty."};
    }

    if(!m_graph->isDirected() || !m_graph->isWeighted()) {
        return AlgorithmError {AlgorithmErrorType::GraphTypeInvalid, "Graph type is invalid."};
    }

    auto nodes = m_graph->getNodes();
    if(nodes.find(start) == nodes.end()) {
        return AlgorithmError {AlgorithmErrorType::StartNodeMissing,
                               "Start node does not exist in the graph."};
    }

    return std::nullopt;
}

std::optional<AlgorithmError> BellmanFord::execute(unsigned idStartNode, unsigned) {

    if(auto err = checkConditions(idStartNode)) {
        return err;
    }

    clearSteps();

    bellmanFord(idStartNode);

    return std::nullopt;
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
        s.m_type = StepType::UpdateDistance;
        s.m_node = start;
        addStep(s);
    }

    unsigned v = nodes.size();

    // relax all edges v-1 times
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
                s.m_type = StepType::ProcessNode; // using as "pass k"
                s.m_node = u;
                addStep(s);
            }
            {
                AlgorithmStep s;
                s.m_type = StepType::ExamineEdge;
                s.m_from = u;
                s.m_to   = v;
                addStep(s);
            }
            if(m_minDistance[u] != std::numeric_limits<int>::max() &&
               m_minDistance[u] + w < m_minDistance[v]) {
                m_minDistance[v] = m_minDistance[u] + w;
                wasRelaxed       = true;
                {
                    AlgorithmStep s;
                    s.m_type = StepType::RelaxEdge;
                    s.m_from = u;
                    s.m_to   = v;
                    addStep(s);
                }
                {
                    AlgorithmStep s;
                    s.m_type = StepType::UpdateDistance;
                    s.m_node = v;
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
        if(dist != std::numeric_limits<int>::max()) {
            res += QString("Node %1 : %2\n").arg(node).arg(dist);
        } else {
            res += QString("Node %1 : unreachable\n").arg(node);
        }
    }

    if(m_hasNegativeCycle) {
        res += "Graph contains a negative cycle!";
    }

    return res;
}
