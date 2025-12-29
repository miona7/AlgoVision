#include "WeightedDirectedGraph.h"

void WeightedDirectedGraph::addEdge(const unsigned from, const unsigned to, const int w) {
    if(m_nodes.find(from) == m_nodes.end() || m_nodes.find(to) == m_nodes.end()) {
        return;
    }
    unsigned edgeId = ++m_numOfEdges;
    m_edges.emplace(edgeId, Edge(edgeId, from, to, w));
    m_adjacencyList[from][edgeId] = to;
}

void WeightedDirectedGraph::removeEdge(const unsigned edgeId) {
    auto it = m_edges.find(edgeId);
    if(it == m_edges.end()) {
        return;
    }
    unsigned from = it->second.startNode();
    m_adjacencyList[from].erase(edgeId);
    m_edges.erase(edgeId);
    --m_numOfEdges;
}
