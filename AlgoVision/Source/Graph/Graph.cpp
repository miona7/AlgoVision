#include "Graph.h"

void Graph::addNode(const unsigned id) {
    if(m_nodes.find(id) != m_nodes.end()) {
        return; // cvor vec postoji
    }
    ++m_numOfNodes;
    m_adjacencyList[id] = {};
    m_nodes.emplace(id, Node(id));
}

void Graph::removeNode(const unsigned id) {
    if(m_nodes.find(id) == m_nodes.end()) {
        return; // cvor ne postoji
    }

    // ukloni sve grane koje izlaze iz cvora
    for (const auto& [edgeId, _] : m_adjacencyList[id]) {
        removeEdge(edgeId);
    }

    // ukloni sve grane koje ulaze u cvor
    for (auto& [nodeId, neighbors] : m_adjacencyList) {
        neighbors.erase(id);
    }

    // obrisi cvor
    m_adjacencyList.erase(id);
    m_nodes.erase(id);

    --m_numOfNodes;
}

std::map<unsigned, std::map<unsigned, unsigned>> Graph::getAdjacencyList() const {
    return m_adjacencyList;
}

std::map<unsigned, Node> Graph::getNodes() const {
    return m_nodes;
}

std::map<unsigned, Edge> Graph::getEdges() const {
    return m_edges;
}
