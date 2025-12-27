#include "UnweightedUndirectedGraph.h"

void UnweightedUndirectedGraph::addEdge(Node* u, Node* v, int w) {
    if(u == nullptr || v == nullptr) {
        return;
    }

    m_edges.push_back(new Edge(u, v, false, w));
}

void UnweightedUndirectedGraph::removeEdge(Node* u, Node* v) {
    if(u == nullptr || v == nullptr) {
        return;
    }
    for(auto it = m_edges.begin(); it != m_edges.end();) {
        Edge* e = *it;
        if((e->startNode() == u && e->endNode() == v) || (e->startNode() == v && e->endNode() == u)) {
            delete e;
            it = m_edges.erase(it);
        } else {
            ++it;
        }
    }
}
