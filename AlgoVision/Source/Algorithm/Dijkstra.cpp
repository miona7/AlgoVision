#include "Dijkstra.h"

Dijkstra::Dijkstra(const std::shared_ptr<Graph> g) : Algorithm(g) {
}

std::optional<AlgorithmError> Dijkstra::checkConditions(unsigned start) const {
    if(m_graph == nullptr || m_graph->getNodes().empty()) {
        return AlgorithmError{AlgorithmErrorType::GraphNotInitialized,
                              "Graph is not initialized or empty."};
    }

    if(!m_graph->isWeighted()) {
        return AlgorithmError{AlgorithmErrorType::GraphTypeInvalid, "Graph type is invalid."};
    }

    auto nodes = m_graph->getNodes();
    if(nodes.find(start) == nodes.end()) {
        return AlgorithmError{AlgorithmErrorType::StartNodeMissing,
                              "Start node does not exist in the graph."};
    }

    auto edges = m_graph->getEdges();
    for(const auto& [_, edge]: edges) {
        if(edge.getWeight() < 0) {
            return AlgorithmError{
                AlgorithmErrorType::NegativeEdgeWeights,
                "Dijkstra cannot be applied to graphs with negative edge weights."};
        }
    }

    return std::nullopt;
}

std::optional<AlgorithmError> Dijkstra::execute(const AlgorithmParams& params) {
    auto idStartNode = params.m_startNode.has_value() ? params.m_startNode.value() : 0;

    if(auto err = checkConditions(idStartNode)) {
        return err;
    }

    clearSteps();

    dijkstra(idStartNode);

    return std::nullopt;
}

void Dijkstra::dijkstra(unsigned start) {
    std::map<unsigned, bool> finished;
    std::map<unsigned, int>  minDistance;

    auto nodes = m_graph->getNodes();
    for(const auto& [nodeId, _]: nodes) {
        finished[nodeId]    = false;
        minDistance[nodeId] = std::numeric_limits<int>::max();
    }

    // min-hip: pair<distance, node>
    std::priority_queue<std::pair<int, unsigned>, std::vector<std::pair<int, unsigned>>,
                        std::greater<>>
        pq;
    pq.emplace(0, start);
    minDistance[start] = 0;

    addStep(AlgorithmStep{StepType::UpdateDistance, start});

    auto adjList = m_graph->getAdjacencyList();
    auto edges   = m_graph->getEdges();

    while(!pq.empty()) {
        auto [currentDistance, currentNode] = pq.top();
        pq.pop();

        if(!finished[currentNode]) {
            finished[currentNode] = true;

            addStep(AlgorithmStep{StepType::VisitNode, currentNode});
            addStep(AlgorithmStep{StepType::ProcessNode, currentNode});

            for(const auto& [edgeId, neighbourId]: adjList[currentNode]) {
                auto it = edges.find(edgeId);
                if(it != edges.end()) {
                    int weight = it->second.getWeight();

                    addStep(AlgorithmStep{StepType::ExamineEdge, std::nullopt, currentNode,
                                          neighbourId});

                    if(currentDistance + weight < minDistance[neighbourId]) {
                        minDistance[neighbourId] = currentDistance + weight;

                        addStep(AlgorithmStep{StepType::UpdateDistance, neighbourId});

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
QString Dijkstra::getResultString() const {
    return m_resultString;
}
