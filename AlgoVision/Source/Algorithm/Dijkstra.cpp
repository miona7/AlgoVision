#include "Dijkstra.h"

Dijkstra::Dijkstra(const std::shared_ptr<Graph> g) : Algorithm(g) {
}

std::optional<AlgorithmError> Dijkstra::checkConditions(unsigned start) const {
    if(m_graph == nullptr || m_graph->getNodes().empty()) {
        return AlgorithmError {AlgorithmErrorType::GraphNotInitialized,
                               "Graph is not initialized or empty."};
    }

    auto nodes = m_graph->getNodes();
    if(nodes.find(start) == nodes.end()) {
        return AlgorithmError {AlgorithmErrorType::StartNodeMissing,
                               "Start node does not exist in the graph."};
    }

    auto edges = m_graph->getEdges();
    for(const auto& [_, edge]: edges) {
        if(edge.getWeight() < 0) {
            return AlgorithmError {AlgorithmErrorType::NegativeEdgeWeights,
                                   "Graph contains edge with negative weight."};
        }
    }

    return std::nullopt;
}

std::optional<AlgorithmError> Dijkstra::execute(unsigned idStartNode, unsigned) {
    if(auto err = checkConditions(idStartNode)) {
        return err;
    }

    clearSteps();
    dijkstra(idStartNode);

    return std::nullopt;
}

void Dijkstra::dijkstra(unsigned start) {
    std::map<unsigned, bool> finished;    // da li smo nasli rastojanje do cvora
    std::map<unsigned, int>  minDistance; // minimalna rastojanja za svaki cvor

    auto nodes = m_graph->getNodes();
    for(const auto& [nodeId, _]: nodes) {
        finished[nodeId]    = false;
        minDistance[nodeId] = std::numeric_limits<int>::max();
    }

    // min-hip: pair<rastojanje, cvor>
    std::priority_queue<std::pair<int, unsigned>, std::vector<std::pair<int, unsigned>>,
                        std::greater<>>
        pq;
    pq.emplace(0, start);
    minDistance[start] = 0;

    {
        AlgorithmStep s;
        s.m_type = StepType::UpdateDistance;
        s.m_node = start;
        addStep(s);
    }

    auto adjList = m_graph->getAdjacencyList();
    auto edges   = m_graph->getEdges();

    while(!pq.empty()) {
        auto [currentDistance, currentNode] = pq.top();
        pq.pop();

        if(!finished[currentNode]) {
            finished[currentNode] = true;

            {
                AlgorithmStep s;
                s.m_type = StepType::VisitNode;
                s.m_node = currentNode;
                addStep(s);
            }
            {
                AlgorithmStep s;
                s.m_type = StepType::ProcessNode;
                s.m_node = currentNode;
                addStep(s);
            }

            for(const auto& [edgeId, neighbourId]: adjList[currentNode]) {
                auto it = edges.find(edgeId);
                if(it != edges.end()) {
                    int weight = it->second.getWeight();

                    {
                        AlgorithmStep s;
                        s.m_type = StepType::ExamineEdge;
                        s.m_from = currentNode;
                        s.m_to   = neighbourId;
                        addStep(s);
                    }

                    if(currentDistance + weight < minDistance[neighbourId]) {
                        minDistance[neighbourId] = currentDistance + weight;

                        {
                            AlgorithmStep s;
                            s.m_type = StepType::UpdateDistance;
                            s.m_node = neighbourId;
                            addStep(s);
                        }

                        pq.emplace(minDistance[neighbourId], neighbourId);
                    }
                }
            }
        }
    }

    m_resultString = "Shortest distances from node " + QString::number(start) + ":\n";

    for(const auto& [id, dist]: minDistance) {
        if(dist == std::numeric_limits<int>::max()) {
            m_resultString += QString("Node %1: unreachable\n").arg(id);
        } else {
            m_resultString += QString("Node %1: %2\n").arg(id).arg(dist);
        }
    }
}
QString Dijkstra::resultString() const {
    return m_resultString;
}
