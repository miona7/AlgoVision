#include "AStar.h"

AStar::AStar(const std::shared_ptr<Graph> g) : Algorithm(g) {
}

std::optional<AlgorithmError> AStar::checkConditions(unsigned start, unsigned goal) const {
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

    if(nodes.find(goal) == nodes.end()) {
        return AlgorithmError{AlgorithmErrorType::GoalNodeMissing,
                              "Goal node does not exist in the graph."};
    }

    auto edges = m_graph->getEdges();
    for(const auto& [id, edge]: edges) {
        if(edge.getWeight() < 0) {
            return AlgorithmError{AlgorithmErrorType::NegativeEdgeWeights,
                                  "A* cannot be applied to graphs with negative edge weights."};
        }
    }

    return std::nullopt;
}

std::optional<AlgorithmError> AStar::execute(const AlgorithmParams& params) {
    auto start = params.m_startNode.has_value() ? params.m_startNode.value() : 0;
    auto goal  = params.m_endNode.has_value() ? params.m_endNode.value() : 0;

    if(auto err = checkConditions(start, goal)) {
        return err;
    }

    clearSteps();

    return aStar(start, goal);
}

std::optional<AlgorithmError> AStar::aStar(unsigned start, unsigned goal) {
    m_path.clear();

    std::set<unsigned> openList = {start};
    std::set<unsigned> closedList;

    std::map<unsigned, int> g; // minimal found cost of path from start to current node
    std::map<unsigned, int> f; // estimated cost from start to goal via current node -> f = g + h
    std::map<unsigned, unsigned> parent;

    auto nodes = m_graph->getNodes();
    for(const auto& [id, _]: nodes) {
        g[id] = std::numeric_limits<int>::max();
        f[id] = std::numeric_limits<int>::max();
    }

    g[start] = 0;
    f[start] = heuristic(start, goal);

    addStep(AlgorithmStep{StepType::UpdateDistance, start});

    // min-heap -> pair<f, node>
    std::priority_queue<std::pair<int, unsigned>, std::vector<std::pair<int, unsigned>>,
                        std::greater<>>
        pq;
    pq.emplace(f[start], start);

    auto adjList = m_graph->getAdjacencyList();
    auto edges   = m_graph->getEdges();

    while(!pq.empty()) {
        auto [_, current] = pq.top();
        pq.pop();

        addStep(AlgorithmStep{StepType::VisitNode, current});
        addStep(AlgorithmStep{StepType::ProcessNode, current});

        if(current == goal) {
            m_totalCost = g[current];

            // path reconstruction
            while(current != start) {
                m_path.push_back(current);

                addStep(AlgorithmStep{StepType::AddToPath, current});
                addStep(
                    AlgorithmStep{StepType::SelectEdge, std::nullopt, parent[current], current});

                current = parent[current];
            }
            m_path.push_back(start);

            addStep(AlgorithmStep{StepType::AddToPath, start});

            std::reverse(m_path.begin(), m_path.end());

            return std::nullopt;
        }

        if(adjList.find(current) != adjList.end()) {
            for(const auto& [edgeId, neighbour]: adjList[current]) {
                auto it = edges.find(edgeId);
                if(it != edges.end()) {
                    addStep(AlgorithmStep{StepType::ExamineEdge, std::nullopt, current, neighbour});

                    if(openList.find(neighbour) == openList.end() &&
                       closedList.find(neighbour) == closedList.end()) {
                        openList.insert(neighbour);
                        parent[neighbour] = current;
                        g[neighbour]      = g[current] + it->second.getWeight();
                        f[neighbour]      = g[neighbour] + heuristic(neighbour, goal);
                        pq.emplace(f[neighbour], neighbour);

                        addStep(AlgorithmStep{StepType::UpdateDistance, neighbour});
                    } else if(g[neighbour] > g[current] + it->second.getWeight()) {
                        parent[neighbour] = current;
                        g[neighbour]      = g[current] + it->second.getWeight();
                        f[neighbour]      = g[neighbour] + heuristic(neighbour, goal);
                        pq.emplace(f[neighbour], neighbour);

                        addStep(AlgorithmStep{StepType::UpdateDistance, neighbour});

                        if(closedList.find(neighbour) != closedList.end()) {
                            closedList.erase(neighbour);
                            openList.insert(neighbour);
                        }
                    }
                }
            }
        }
        openList.erase(current);
        closedList.insert(current);
    }

    return AlgorithmError{AlgorithmErrorType::NoPathFound,
                          "No path exists between start and goal nodes."};
}

double AStar::calculateScalingFactor() const {
    auto edges = m_graph->getEdges();
    if(edges.empty()) {
        return 1.0;
    }

    double totalDistance = 0.0;
    int    count         = 0;

    auto nodes = m_graph->getNodes();
    for(const auto& [id, edge]: edges) {
        auto u = nodes.find(edge.startNode());
        auto v = nodes.find(edge.endNode());

        if(u != nodes.end() && v != nodes.end()) {
            auto [x1, y1] = u->second.getPosition();
            auto [x2, y2] = v->second.getPosition();

            // euclidian distance beetwen nodes in pixels
            totalDistance += std::sqrt(std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2));
            count++;
        }
    }

    // average distance in pixels
    return (count > 0) ? (totalDistance / count) : 1.0;
}

int AStar::heuristic(unsigned node, unsigned goal) const {
    auto nodes  = m_graph->getNodes();
    auto itNode = nodes.find(node);
    auto itGoal = nodes.find(goal);

    if(itNode != nodes.end() && itGoal != nodes.end()) {
        auto [x1, y1] = itNode->second.getPosition();
        auto [x2, y2] = itGoal->second.getPosition();

        double rawDist = std::sqrt(std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2));
        double factor  = calculateScalingFactor();

        // normalise value
        return static_cast<int>(rawDist / factor);
    }

    return 0; // fallback heuristic
}

const std::vector<unsigned>& AStar::getPath() const {
    return m_path;
}

int AStar::getTotalCost() const {
    return m_totalCost;
}

QString AStar::getResultString() const {
    return QString("Total path cost: %1").arg(m_totalCost);
}
