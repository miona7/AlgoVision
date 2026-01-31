#include "BFS.h"

BFS::BFS(const std::shared_ptr<Graph> g) : Algorithm(g) {
}

void BFS::checkConditions(unsigned start) const {
    if(!m_graph || m_graph->getNodes().empty()) {
        throw std::runtime_error("Graph is not initialized or invalid!");
    }

    auto nodes = m_graph->getNodes();
    if(nodes.find(start) == nodes.end()) {
        throw std::runtime_error("Start node does not exist in graph!");
    }
}

void BFS::execute(unsigned idStartNode, unsigned) {
    checkConditions(idStartNode);

    clearSteps();

    bfs(idStartNode);
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

        if(adjList.find(current) != adjList.end()) {
            for(const auto& [_, neighbourId]: adjList[current]) {

                {
                    AlgorithmStep s;
                    s.m_type = StepType::ExamineEdge;
                    s.m_from = current;
                    s.m_to   = neighbourId;
                    addStep(s);
                }

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
    for(unsigned v : m_order)
        res += QString::number(v) + " ";
    return res;
}
