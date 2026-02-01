#include "DFS.h"

DFS::DFS(const std::shared_ptr<Graph> g) : Algorithm(g) {
}

std::optional<AlgorithmError> DFS::checkConditions(unsigned start) const {
    if(m_graph == nullptr  || m_graph->getNodes().empty()) {
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

    return std::nullopt;
}

std::optional<AlgorithmError> DFS::execute(unsigned idStartNode, unsigned) {
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

    {
        AlgorithmStep s;
        s.m_type = StepType::VisitNode;
        s.m_node = nodeId;
        addStep(s);
    }
    {
        AlgorithmStep s;
        s.m_type = StepType::ProcessNode;
        s.m_node = nodeId;
        addStep(s);
    }

    auto adjList = m_graph->getAdjacencyList();
    if(adjList.find(nodeId) != adjList.end()) {
        for(const auto& [_, neighbourId]: adjList[nodeId]) {

            {
                AlgorithmStep s;
                s.m_type = StepType::ExamineEdge;
                s.m_from = nodeId;
                s.m_to   = neighbourId;
                addStep(s);
            }

            if(!m_visited[neighbourId]) {
                dfs(neighbourId);
            }
        }
    }
}

const std::map<unsigned, bool>& DFS::getVisited() const {
    return m_visited;
}
