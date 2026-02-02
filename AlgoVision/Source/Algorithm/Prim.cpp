#include "Prim.h"

Prim::Prim(const std::shared_ptr<Graph> g) : Algorithm(g) {
}

std::optional<AlgorithmError> Prim::checkConditions() const {
    if(m_graph == nullptr || m_graph->getNodes().empty()) {
        return AlgorithmError {AlgorithmErrorType::GraphNotInitialized,
                               "Graph is not initialized or empty."};
    }

    if(m_graph->isDirected()) {
        return AlgorithmError {AlgorithmErrorType::GraphTypeInvalid, "Graph type is invalid."};
    }

    BFS      bfs(m_graph);
    unsigned start = m_graph->getNodes().begin()->first;

    if(auto err = bfs.execute(start)) {
        return err;
    }

    for(const auto& [_, visited]: bfs.getVisited()) {
        if(!visited) {
            return AlgorithmError {AlgorithmErrorType::GraphNotConnected, "Graph is not connected"};
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
    std::map<unsigned, bool> inTree;      // da li je cvor vec u drvetu
    std::map<unsigned, int>  minDistance; // minimalno rastojanje cvora do drveta
    std::map<unsigned, std::optional<unsigned>>
        parent; // za svaki cvor pamtimo iz kog cvora smo dosli do njeg

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

    {
        AlgorithmStep s;
        s.m_type = StepType::UpdateDistance;
        s.m_node = start;
        addStep(s);
    }

    auto adjList = m_graph->getAdjacencyList();
    auto edges   = m_graph->getEdges();

    while(!pq.empty()) {
        auto [_, currentNode] = pq.top();
        pq.pop();

        if(!inTree[currentNode]) {
            inTree[currentNode] = true;

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

            if(parent[currentNode]) {
                AlgorithmStep s;
                s.m_type = StepType::SelectEdge;
                s.m_from = parent[currentNode];
                s.m_to   = currentNode;
                addStep(s);
            }

            for(const auto& [edgeId, neighbourId]: adjList[currentNode]) {
                if(!inTree[neighbourId]) {
                    {
                        AlgorithmStep s;
                        s.m_type = StepType::ExamineEdge;
                        s.m_from = currentNode;
                        s.m_to   = neighbourId;
                        addStep(s);
                    }
                    auto it = edges.find(edgeId);
                    if(it != edges.end()) {
                        int weight = it->second.getWeight();
                        if(weight < minDistance[neighbourId]) {
                            minDistance[neighbourId] = weight;
                            parent[neighbourId]      = currentNode;
                            pq.emplace(minDistance[neighbourId], neighbourId);
                            {
                                AlgorithmStep s;
                                s.m_type = StepType::UpdateDistance;
                                s.m_node = neighbourId;
                                addStep(s);
                            }
                        }
                    }
                }
            }
        }
    }

    int totalWeight = 0;

    for(const auto& [u, v]: parent) {
        if(v) {
            totalWeight += minDistance[u];
        }
    }

    std::cout << "Total weight of MST is " << totalWeight << std::endl;
}
