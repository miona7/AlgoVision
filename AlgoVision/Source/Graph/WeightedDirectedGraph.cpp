#include "WeightedDirectedGraph.h"

void WeightedDirectedGraph::addEdge(unsigned from, unsigned to, int w) {
    if(m_nodes.find(from) == m_nodes.end() || m_nodes.find(to) == m_nodes.end()) {
        return;
    }
    unsigned edgeId = m_edgeId++;
    ++m_numOfEdges;
    m_edges.emplace(edgeId, Edge(edgeId, from, to, w));
    m_adjacencyList[from][edgeId] = to;
}

void WeightedDirectedGraph::removeEdge(unsigned edgeId) {
    auto it = m_edges.find(edgeId);
    if(it == m_edges.end()) {
        return;
    }

    for(auto& [_, neighbour]: m_adjacencyList) {
        neighbour.erase(edgeId);
    }

    m_edges.erase(edgeId);
    --m_numOfEdges;
}

bool WeightedDirectedGraph::isDirected() const {
    return true;
}

bool WeightedDirectedGraph::isWeighted() const {
    return true;
}
