#include "Kahn.h"

Kahn::Kahn(const std::shared_ptr<Graph> g) : Algorithm(g) {
}

std::optional<AlgorithmError> Kahn::checkConditions() const {
    if(m_graph == nullptr || m_graph->getNodes().empty()) {
        return AlgorithmError{
            AlgorithmErrorType::GraphNotInitialized,
            "Graph is not initialized or empty."
        };
    }

    if(!m_graph->isDirected()) {
        return AlgorithmError{
            AlgorithmErrorType::GraphTypeInvalid,
            "Graph type is invalid."
        };
    }

    return std::nullopt;
}

std::optional<AlgorithmError> Kahn::execute(unsigned, unsigned) {
    if(auto err = checkConditions()) {
        return err;
    }

    clearSteps();

    if(auto err = kahn()) {
        return err;
    }

    std::cout << "Topological order:" << std::endl;
    for(unsigned node: m_sorted) {
        std::cout << node << " ";
    }
    std::cout << std::endl;

    return std::nullopt;
}

std::optional<AlgorithmError> Kahn::kahn() {
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
        return AlgorithmError{
            AlgorithmErrorType::GraphHasCycle,
            "Graph contains a cycle."
        };
    }

    return std::nullopt;
}

const std::vector<unsigned>& Kahn::getSorted() const {
    return m_sorted;
}
