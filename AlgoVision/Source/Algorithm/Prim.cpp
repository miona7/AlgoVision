#include "Prim.h"

Prim::Prim(const std::shared_ptr<Graph> g) : Algorithm(g) {
}

void Prim::checkConditions() const {
    if(!m_graph || m_graph->getNodes().empty() || m_graph->isDirected()) {
        throw std::runtime_error("Graph is not initialized or invalid!");
    }

    BFS      bfs(m_graph);
    unsigned start = m_graph->getNodes().begin()->first;
    bfs.execute(start);

    for(const auto& [_, visited]: bfs.getVisited()) {
        if(!visited) {
            throw std::runtime_error("Graph is not connected!");
        }
    }
}

void Prim::execute(unsigned, unsigned) {
    checkConditions();

    clearSteps();
    {
        AlgorithmStep s;
        s.m_type    = StepType::Start;
        s.m_message = std::string("Prim start");
        addStep(s);
    }

    std::cout << "Starting Prim." << std::endl;
    prim();
    std::cout << "Prim finished." << std::endl;

    {
        AlgorithmStep s;
        s.m_type    = StepType::Finish;
        s.m_message = std::string("Prim finish");
        addStep(s);
    }
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
        s.m_type = StepType::VisitNode;
        s.m_node = start;
        addStep(s);
    }
    {
        AlgorithmStep s;
        s.m_type  = StepType::UpdateDistance;
        s.m_node  = start;
        s.m_value = 0;
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
                s.m_type = StepType::ProcessNode;
                s.m_node = currentNode;
                addStep(s);
            }
            {
                AlgorithmStep s;
                s.m_type  = StepType::MarkNode;
                s.m_node  = currentNode;
                s.m_value = 1; // inTree = true
                addStep(s);
            }

            if(parent[currentNode]) {
                AlgorithmStep s;
                s.m_type  = StepType::SelectEdge;
                s.m_from  = parent[currentNode];
                s.m_to    = currentNode;
                s.m_value = minDistance[currentNode]; // težina ivice
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
                                s.m_type  = StepType::UpdateDistance;
                                s.m_node  = neighbourId;
                                s.m_value = weight;
                                addStep(s);
                            }
                        }
                    }
                }
            }
        }
    }

    int totalWeight = 0;

    std::cout << "Minimum Spanning Tree:" << std::endl;
    for(const auto& [u, v]: parent) {
        if(v) {
            std::cout << *v << " -> " << u << " edge = " << minDistance[u] << std::endl;
            totalWeight += minDistance[u];
        }
    }

    std::cout << "Total weight of MST is " << totalWeight << std::endl;
}
