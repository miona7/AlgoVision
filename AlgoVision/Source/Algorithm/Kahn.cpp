#include "Kahn.h"

Kahn::Kahn(const std::shared_ptr<Graph> g) : Algorithm(g) {
}

void Kahn::checkConditions() const {
    if(!m_graph || m_graph->getNodes().empty() || !m_graph->isDirected()) {
        throw std::runtime_error("Graph is not initialized or invalid!");
    }
}

void Kahn::execute(unsigned, unsigned) {
    checkConditions();
    clearSteps();
    kahn();
    std::cout << "Topological order:" << std::endl;
    for(unsigned node: m_sorted) {
        std::cout << node << " ";
    }
    std::cout << std::endl;
}

void Kahn::kahn() {
    m_sorted.clear();

    auto                         nodes = m_graph->getNodes();
    std::map<unsigned, unsigned> inDegree; // ulazni stepen svakog cvora
    for(const auto& [id, _]: nodes) {
        inDegree[id] = 0;
    }

    auto adjList = m_graph->getAdjacencyList();
    for(const auto& [u, row]: adjList) {
        for(const auto& [edge, v]: row) {
            inDegree[v]++;
        }
    }

    std::queue<unsigned> q;
    for(const auto& [u, deg]: inDegree) {
        if(deg == 0) {
            q.push(u);
        }
    }

    while(!q.empty()) {
        unsigned node = q.front();
        q.pop();

        m_sorted.push_back(node);

        {
            AlgorithmStep s;
            s.m_type = StepType::ProcessNode;
            s.m_node = node;
            addStep(s);
        }
        {
            AlgorithmStep s;
            s.m_type = StepType::AddToTopologicalOrder;
            s.m_node = node;
            addStep(s);
        }

        if(adjList.find(node) != adjList.end()) {
            for(const auto& [edge, neighbour]: adjList[node]) {
                inDegree[neighbour]--;
                if(inDegree[neighbour] == 0) {
                    q.push(neighbour);
                }
            }
        }
    }

    if(m_sorted.size() != nodes.size()) {
        throw std::runtime_error("Graph contains a cycle, topological sort not possible!");
    }
}

const std::vector<unsigned>& Kahn::getSorted() const {
    return m_sorted;
}
