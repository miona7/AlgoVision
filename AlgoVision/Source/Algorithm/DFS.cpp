#include "DFS.h"

DFS::DFS(const std::shared_ptr<Graph> g) : Algorithm(g) {
}

void DFS::checkConditions(unsigned start) const {
    // graf postoji i ima bar 1 cvor

    if(!m_graph || m_graph->getNodes().empty()) {
        throw std::runtime_error("Graph is not initialized or invalid!");
    }

    auto nodes = m_graph->getNodes();
    if(nodes.find(start) == nodes.end()) {
        throw std::runtime_error("Start node does not exist in graph!");
    }
}

void DFS::execute(unsigned idStartNode, unsigned) {
    checkConditions(idStartNode);

    clearSteps();
    m_visited.clear();
    auto nodes = m_graph->getNodes();
    for(const auto& [id, _]: nodes) {
        m_visited[id] = false;
    }

    dfs(idStartNode);
}

void DFS::dfs(unsigned nodeId) {
    m_visited[nodeId] = true;

    m_order.push_back(nodeId);
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

QString DFS::resultString() const {
    QString res = "DFS order: ";
    for(unsigned v: m_order) {
        res += QString::number(v) + " ";
    }
    return res;
}
