#include "DFS.h"

DFS::DFS(const std::shared_ptr<Graph> g) : Algorithm(g) {
}

std::optional<AlgorithmError> DFS::checkConditions(unsigned start) const {
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

std::optional<AlgorithmError> DFS::execute(const AlgorithmParams& params) {
    auto idStartNode = params.m_startNode.has_value() ? params.m_startNode.value() : 0;

    if(auto err = checkConditions(idStartNode)) {
        return err;
    }

    clearSteps();
    m_visited.clear();
    auto nodes = m_graph->getNodes();
    for(const auto& [id, _]: nodes) {
        m_visited[id] = false;
    }

    dfs(idStartNode);

    return std::nullopt;
}

void DFS::dfs(unsigned nodeId) {
    m_visited[nodeId] = true;

    m_order.push_back(nodeId);

    addStep(AlgorithmStep{StepType::VisitNode, nodeId});
    addStep(AlgorithmStep{StepType::ProcessNode, nodeId});

    auto adjList = m_graph->getAdjacencyList();
    if(adjList.find(nodeId) != adjList.end()) {
        for(const auto& [_, neighbourId]: adjList[nodeId]) {
            addStep(AlgorithmStep{StepType::ExamineEdge, std::nullopt, nodeId, neighbourId});

            if(!m_visited[neighbourId]) {
                dfs(neighbourId);
            }
        }
    }
}

const std::map<unsigned, bool>& DFS::getVisited() const {
    return m_visited;
}

QString DFS::getResultString() const {
    QString res = "DFS order: ";
    for(unsigned v: m_order) {
        res += QString::number(v) + " ";
    }
    return res;
}
