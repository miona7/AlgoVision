#include "Kahn.h"

Kahn::Kahn(const std::shared_ptr<Graph> g) : Algorithm(g) {
}

std::optional<AlgorithmError> Kahn::checkConditions() const {
    if(m_graph == nullptr || m_graph->getNodes().empty()) {
        return AlgorithmError{AlgorithmErrorType::GraphNotInitialized,
                              "Graph is not initialized or empty."};
    }

    if(!m_graph->isDirected()) {
        return AlgorithmError{AlgorithmErrorType::GraphTypeInvalid, "Graph type is invalid."};
    }

    return std::nullopt;
}

std::optional<AlgorithmError> Kahn::execute(const AlgorithmParams&) {
    if(auto err = checkConditions()) {
        return err;
    }

    clearSteps();

    if(auto err = kahn()) {
        return err;
    }

    return std::nullopt;
}

std::optional<AlgorithmError> Kahn::kahn() {
    m_sorted.clear();

    auto                         nodes = m_graph->getNodes();
    std::map<unsigned, unsigned> inDegree;
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

        addStep(AlgorithmStep{StepType::ProcessNode, node});
        addStep(AlgorithmStep{StepType::AddToTopologicalOrder, node});

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
        return AlgorithmError{AlgorithmErrorType::GraphHasCycle, "Graph contains a cycle."};
    }

    return std::nullopt;
}

const std::vector<unsigned>& Kahn::getSorted() const {
    return m_sorted;
}

QString Kahn::getResultString() const {
    QString result = "Topological order:\n";

    for(size_t i = 0; i < m_sorted.size(); ++i) {
        result += QString::number(m_sorted[i]);
        if(i + 1 < m_sorted.size()) {
            result += " -> ";
        }
    }

    return result;
}
