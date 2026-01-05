#include "Dijkstra.h"

Dijkstra::Dijkstra(const std::shared_ptr<Graph>& g) : Algorithm(g) {
}

void Dijkstra::checkConditions(unsigned start) const {
    if(!m_graph || m_graph->getNodes().empty()) {
        throw std::runtime_error("Graph is not initialized or invalid!");
    }

    auto nodes = m_graph->getNodes();
    if(nodes.find(start) == nodes.end()) {
        throw std::runtime_error("Start node does not exist in graph!");
    }

    auto edges = m_graph->getEdges();
    for(const auto& [_, edge]: edges) {
        if(edge.getWeight() < 0) {
            throw std::runtime_error("Graph contains edge with negative weight!");
        }
    }
}

void Dijkstra::execute(unsigned idStartNode, unsigned) {
    checkConditions(idStartNode);

    clearSteps();

    {
        AlgorithmStep s;
        s.m_type = StepType::Start;
        s.m_node = idStartNode;
        s.m_message = std::string("Dijkstra start");
        addStep(s);
    }

    // std::cout << "Starting Dijkstra." << std::endl;
    dijkstra(idStartNode);
    // std::cout << "Dijkstra finished." << std::endl;

    {
        AlgorithmStep s;
        s.m_type = StepType::Finish;
        s.m_message = std::string("Dijkstra finish");
        addStep(s);
    }
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
        s.m_type = StepType::PushToQueue;
        s.m_node = start;
        addStep(s);
    }

    auto adjList = m_graph->getAdjacencyList();
    auto edges   = m_graph->getEdges();

    while(!pq.empty()) {
        auto [currentDistance, currentNode] = pq.top();
        pq.pop();

        {
            AlgorithmStep s;
            s.m_type = StepType::PopFromQueue;
            s.m_node = currentNode;
            addStep(s);
        }

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
                        s.m_to = neighbourId;
                        s.m_value = weight;
                        addStep(s);
                    }

                    if(currentDistance + weight < minDistance[neighbourId]) {
                        minDistance[neighbourId] = currentDistance + weight;

                        {
                            AlgorithmStep s;
                            s.m_type = StepType::UpdateDistance;
                            s.m_node = neighbourId;
                            s.m_value = minDistance[neighbourId];
                            addStep(s);
                        }

                        pq.emplace(minDistance[neighbourId], neighbourId);

                        {
                            AlgorithmStep s;
                            s.m_type = StepType::PushToQueue;
                            s.m_node = neighbourId;
                            addStep(s);
                        }
                    }
                }
            }
        }
    }

    std::cout << "Shortest distances from node " << start << ":" << std::endl;
    for(const auto& [id, dist]: minDistance) {
        std::cout << "Node " << id << ": ";
        if(dist == std::numeric_limits<int>::max()) {
            std::cout << "unreachable" << std::endl;
        } else {
            std::cout << dist << std::endl;
        }
    }
}
