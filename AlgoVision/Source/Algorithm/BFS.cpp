#include "BFS.h"

BFS::BFS(const std::shared_ptr<Graph>& g) : Algorithm(g) {
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
    {
        AlgorithmStep s;
        s.m_type    = StepType::Start;
        s.m_node    = idStartNode;
        s.m_message = std::string("BFS start");
        addStep(s);
    }

    std::cout << "BFS traversal starting from node " << idStartNode << ":" << std::endl;
    bfs(idStartNode);
    std::cout << "BFS finished." << std::endl;

    {
        AlgorithmStep s;
        s.m_type    = StepType::Finish;
        s.m_message = std::string("BFS finish");
        addStep(s);
    }
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

    {
        AlgorithmStep s;
        s.m_type = StepType::VisitNode;
        s.m_node = start;
        addStep(s);
    }
    {
        AlgorithmStep s;
        s.m_type  = StepType::MarkNode;
        s.m_node  = start;
        s.m_value = 1;
        addStep(s);
    }
    {
        AlgorithmStep s;
        s.m_type = StepType::PushToQueue;
        s.m_node = start;
        addStep(s);
    }

    while(!q.empty()) {
        unsigned current = q.front();
        q.pop();

        {
            AlgorithmStep s;
            s.m_type = StepType::PopFromQueue;
            s.m_node = current;
            addStep(s);
        }
        {
            AlgorithmStep s;
            s.m_type = StepType::ProcessNode;
            s.m_node = current;
            addStep(s);
        }

        std::cout << "visiting node with id " << current << std::endl;
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

                    {
                        AlgorithmStep s;
                        s.m_type = StepType::VisitNode;
                        s.m_node = neighbourId;
                        s.m_from = current; // roditelj koji ga je otkrio
                        addStep(s);
                    }
                    {
                        AlgorithmStep s;
                        s.m_type  = StepType::MarkNode;
                        s.m_node  = neighbourId;
                        s.m_value = 1;
                        addStep(s);
                    }
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

const std::map<unsigned, bool>& BFS::getVisited() const {
    return m_visited;
}
