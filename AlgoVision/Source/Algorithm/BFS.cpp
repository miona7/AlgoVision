#include "BFS.h"

BFS::BFS(const std::shared_ptr<Graph> g) : Algorithm(g) {
}

std::optional<AlgorithmError> BFS::checkConditions(unsigned start) const {
    if(m_graph == nullptr || m_graph->getNodes().empty()) {
        return AlgorithmError{AlgorithmErrorType::GraphNotInitialized,
                              "Graph is not initialized or empty."};
    }

    auto nodes = m_graph->getNodes();
    if(nodes.find(start) == nodes.end()) {
        return AlgorithmError{AlgorithmErrorType::StartNodeMissing,
                              "Start node does not exist in the graph."};
    }

    return std::nullopt;
}

std::optional<AlgorithmError> BFS::execute(unsigned idStartNode, unsigned) {
    if(auto err = checkConditions(idStartNode)) {
        return err;
    }

    clearSteps();

    bfs(idStartNode);

    return std::nullopt;
}

void BFS::bfs(unsigned start) {
    m_visited.clear();

    auto nodes = m_graph->getNodes();
    for(const auto& [id, _]: nodes) {
        m_visited[id] = false;
    }

    auto adjList = m_graph->getAdjacencyList();

    m_visited[start] = true;
    std::queue<unsigned> q;
    q.push(start);

    while(!q.empty()) {
        unsigned current = q.front();
        m_order.push_back(current);

        q.pop();

        addStep(AlgorithmStep{StepType::VisitNode, current});
        addStep(AlgorithmStep{StepType::ProcessNode, current});

        if(adjList.find(current) != adjList.end()) {
            for(const auto& [_, neighbourId]: adjList[current]) {
                addStep(AlgorithmStep{StepType::ExamineEdge, std::nullopt, current, neighbourId});

                if(!m_visited[neighbourId]) {
                    m_visited[neighbourId] = true;
                    q.push(neighbourId);
                }
            }
        }
    }
}

const std::map<unsigned, bool>& BFS::getVisited() const {
    return m_visited;
}

QString BFS::resultString() const {
    QString res = "BFS order: ";
    for(unsigned v: m_order) {
        res += QString::number(v) + " ";
    }
    return res;
}
