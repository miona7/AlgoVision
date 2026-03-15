#include "Prim.h"

Prim::Prim(const std::shared_ptr<Graph> g) : Algorithm(g) {
}

std::optional<AlgorithmError> Prim::checkConditions() const {
    if(m_graph == nullptr || m_graph->getNodes().empty()) {
        return AlgorithmError{AlgorithmErrorType::GraphNotInitialized,
                              "Graph is not initialized or empty."};
    }

    if(m_graph->isDirected() || !m_graph->isWeighted()) {
        return AlgorithmError{AlgorithmErrorType::GraphTypeInvalid, "Graph type is invalid."};
    }

    BFS      bfs(m_graph);
    unsigned start = m_graph->getNodes().begin()->first;

    if(auto err = bfs.execute(start)) {
        return err;
    }

    for(const auto& [_, visited]: bfs.getVisited()) {
        if(!visited) {
            return AlgorithmError{AlgorithmErrorType::GraphNotConnected, "Graph is not connected"};
        }
    }

    return std::nullopt;
}

std::optional<AlgorithmError> Prim::execute(unsigned, unsigned) {
    if(auto err = checkConditions()) {
        return err;
    }

    clearSteps();

    prim();

    return std::nullopt;
}

void Prim::prim() {
    std::map<unsigned, bool>                    inTree;
    std::map<unsigned, int>                     minDistance;
    std::map<unsigned, std::optional<unsigned>> parent; // from which node we came

    auto nodes = m_graph->getNodes();
    for(const auto& [nodeId, _]: nodes) {
        inTree[nodeId]      = false;
        minDistance[nodeId] = std::numeric_limits<int>::max();
        parent[nodeId]      = std::nullopt;
    }

    std::priority_queue<std::pair<int, unsigned>, std::vector<std::pair<int, unsigned>>,
                        std::greater<>>
             pq;
    unsigned start = nodes.begin()->first;
    pq.emplace(0, start);
    minDistance[start] = 0;

    addStep(AlgorithmStep{StepType::UpdateDistance, start});

    auto adjList = m_graph->getAdjacencyList();
    auto edges   = m_graph->getEdges();

    while(!pq.empty()) {
        auto [_, currentNode] = pq.top();
        pq.pop();

        if(!inTree[currentNode]) {
            inTree[currentNode] = true;

            addStep(AlgorithmStep{StepType::VisitNode, currentNode});
            addStep(AlgorithmStep{StepType::ProcessNode, currentNode});

            if(parent[currentNode]) {
                addStep(AlgorithmStep{StepType::SelectEdge, std::nullopt, parent[currentNode], currentNode});
            }

            for(const auto& [edgeId, neighbourId]: adjList[currentNode]) {
                if(!inTree[neighbourId]) {
                    addStep(AlgorithmStep{StepType::ExamineEdge, std::nullopt, currentNode, neighbourId});

                    auto it = edges.find(edgeId);
                    if(it != edges.end()) {
                        int weight = it->second.getWeight();
                        if(weight < minDistance[neighbourId]) {
                            minDistance[neighbourId] = weight;
                            parent[neighbourId]      = currentNode;
                            pq.emplace(minDistance[neighbourId], neighbourId);

                            addStep(AlgorithmStep{StepType::UpdateDistance, neighbourId});
                        }
                    }
                }
            }
        }
    }

    m_totalWeight = 0;

    for(const auto& [u, v]: parent) {
        if(v) {
            m_totalWeight += minDistance[u];
        }
    }
}

QString Prim::resultString() const {
    return QString("Total MST weight: %1").arg(m_totalWeight);
}
