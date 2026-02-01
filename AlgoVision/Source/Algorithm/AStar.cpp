#include "AStar.h"

AStar::AStar(const std::shared_ptr<Graph> g) : Algorithm(g) {
}

std::optional<AlgorithmError> AStar::checkConditions(unsigned start, unsigned goal) const {
    if(m_graph == nullptr || m_graph->getNodes().empty()) {
        return AlgorithmError{
            AlgorithmErrorType::GraphNotInitialized,
            "Graph is not initialized or empty."
        };
    }

    auto nodes = m_graph->getNodes();
    if(nodes.find(start) == nodes.end()) {
        return AlgorithmError{
            AlgorithmErrorType::StartNodeMissing,
            "Start node does not exist in the graph."
        };
    }

    if(nodes.find(goal) == nodes.end()) {
        return AlgorithmError{
            AlgorithmErrorType::GoalNodeMissing,
            "Goal node does not exist in the graph."
        };
    }

    auto edges = m_graph->getEdges();
    for(const auto& [id, edge]: edges) {
        if(edge.getWeight() < 0) {
            return AlgorithmError{
                AlgorithmErrorType::NegativeEdgeWeights,
                "A* cannot be applied to graphs with negative edge weights."
            };
        }
    }

    return std::nullopt;
}


std::optional<AlgorithmError> AStar::execute(unsigned start, unsigned goal) {

    if(auto err = checkConditions(start, goal)) {
        return err;
    }

    clearSteps();

    if(auto err = aStar(start, goal)) {
        return err;
    }

    std::cout << "Path: ";
    for(int i = 0; i < m_path.size(); i++) {
        std::cout << m_path[i];
        if(i != m_path.size() - 1) {
            std::cout << " -> ";
        }
    }
    std::cout << std::endl << "Total cost: " << m_totalCost << std::endl;

    return std::nullopt;
}

std::optional<AlgorithmError> AStar::aStar(unsigned start, unsigned goal) {
    m_path.clear();
    std::map<unsigned, int> gScore; // stvarni trosak puta od startnog do trenutnog cvora
    std::map<unsigned, int> fScore; // procena ukupnog troska od startnog do ciljnog preko trenutnog
    std::map<unsigned, unsigned> parent;
    std::map<unsigned, bool>     visited;

    auto nodes = m_graph->getNodes();
    for(const auto& [id, _]: nodes) {
        gScore[id]  = std::numeric_limits<int>::max();
        fScore[id]  = std::numeric_limits<int>::max();
        visited[id] = false;
    }

    gScore[start] = 0;
    fScore[start] = heuristic(start, goal);

    {
        AlgorithmStep s;
        s.m_type  = StepType::UpdateDistance;
        s.m_node  = start;
        s.m_value = fScore[start];
        addStep(s);
    }

    std::priority_queue<std::pair<int, unsigned>, std::vector<std::pair<int, unsigned>>,
                        std::greater<>>
        pq;
    pq.emplace(fScore[start], start);
    auto adjList = m_graph->getAdjacencyList();
    auto edges   = m_graph->getEdges();

    while(!pq.empty()) {
        auto [_, current] = pq.top();
        pq.pop();

        if(visited[current]) {
            continue;
        }

        visited[current] = true;

        {
            AlgorithmStep s;
            s.m_type = StepType::VisitNode;
            s.m_node = current;
            addStep(s);
        }
        {
            AlgorithmStep s;
            s.m_type = StepType::ProcessNode;
            s.m_node = current;
            addStep(s);
        }

        if(current == goal) {
            // rekonstruisemo put
            m_totalCost = gScore[current];
            while(current != start) {
                m_path.push_back(current);

                {
                    AlgorithmStep s;
                    s.m_type = StepType::AddToPath;
                    s.m_node = current;
                    addStep(s);
                }

                {
                    AlgorithmStep s;
                    s.m_type = StepType::SelectEdge;
                    s.m_from = parent[current];
                    s.m_to   = current;
                    addStep(s);
                }

                current = parent[current];
            }

            m_path.push_back(start);

            {
                AlgorithmStep s;
                s.m_type = StepType::AddToPath;
                s.m_node = start;
                addStep(s);
            }

            std::reverse(m_path.begin(), m_path.end());
            return std::nullopt;
        }

        if(adjList.find(current) != adjList.end()) {
            for(const auto& [edgeId, neighbour]: adjList[current]) {
                auto it = edges.find(edgeId);
                if(it != edges.end()) {

                    {
                        AlgorithmStep s;
                        s.m_type = StepType::ExamineEdge;
                        s.m_from = current;
                        s.m_to   = neighbour;
                        addStep(s);
                    }

                    int tentativeG = gScore[current] + it->second.getWeight();

                    if(tentativeG < gScore[neighbour]) {
                        parent[neighbour] = current;
                        gScore[neighbour] = tentativeG;
                        fScore[neighbour] = tentativeG + heuristic(neighbour, goal);

                        {
                            AlgorithmStep s;
                            s.m_type  = StepType::UpdateDistance;
                            s.m_node  = neighbour;
                            s.m_value = fScore[neighbour];
                            addStep(s);
                        }

                        pq.emplace(fScore[neighbour], neighbour);
                    }
                }
            }
        }
    }

    return AlgorithmError{
        AlgorithmErrorType::NoPathFound,
        "No path exists between start and goal nodes."
    };

}

int AStar::heuristic(unsigned node, unsigned goal) const {
    auto nodes  = m_graph->getNodes();
    auto itNode = nodes.find(node);
    auto itGoal = nodes.find(goal);

    if(itNode != nodes.end() && itGoal != nodes.end()) {
        auto [x1, y1] = itNode->second.getPosition();
        auto [x2, y2] = itGoal->second.getPosition();

        return static_cast<int>(std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)));
    }

    return 0; // fallback heuristika
}

const std::vector<unsigned>& AStar::getPath() const {
    return m_path;
}

int AStar::getTotalCost() const {
    return m_totalCost;
}
