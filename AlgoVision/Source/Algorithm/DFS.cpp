#include "DFS.h"

DFS::DFS(const std::shared_ptr<Graph>& g) : Algorithm(g) {
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

    {
        AlgorithmStep s;
        s.m_type    = StepType::Start;
        s.m_node    = idStartNode;
        s.m_message = std::string("DFS Start");
        addStep(s);
    }

    m_visited.clear();
    auto nodes = m_graph->getNodes();
    for(const auto& [id, _]: nodes) {
        m_visited[id] = false;
    }

    // std::cout << "DFS traversal starting from node " << idStartNode << ":" << std::endl;
    dfs(idStartNode, std::nullopt);
    // std::cout << "DFS finished." << std::endl;

    {
        AlgorithmStep s;
        s.m_type    = StepType::Finish;
        s.m_message = std::string("DFS Finish");
        addStep(s);
    }
}

void DFS::dfs(unsigned nodeId, std::optional<unsigned> parent) {
    m_visited[nodeId] = true;
    // std::cout << "visiting node with id " << nodeId << std::endl;

    {
        AlgorithmStep s;
        s.m_type = StepType::VisitNode;
        s.m_node = nodeId;
        if(parent.has_value()) {
            s.m_from = parent.value();
        }
        addStep(s);
    }

    {
        AlgorithmStep s;
        s.m_type  = StepType::MarkNode;
        s.m_node  = nodeId;
        s.m_value = 1; // visited = true
        addStep(s);
    }

    {
        AlgorithmStep s;
        s.m_type = StepType::PushToStack;
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
                dfs(neighbourId, nodeId);
            }
        }
    }

    {
        AlgorithmStep s;
        s.m_type = StepType::ProcessNode;
        s.m_node = nodeId;
        addStep(s);
    }

    {
        AlgorithmStep s;
        s.m_type = StepType::PopFromStack;
        s.m_node = nodeId;
        addStep(s);
    }
}

const std::map<unsigned, bool>& DFS::getVisited() const {
    return m_visited;
}
