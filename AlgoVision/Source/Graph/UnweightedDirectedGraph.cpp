#include "UnweightedDirectedGraph.h"

void UnweightedDirectedGraph::addEdge(Node* u, Node* v, int w) {
    if(u == nullptr || v == nullptr) {
        return;
    }

    m_edges.push_back(new Edge(u, v, true, w));
}

void UnweightedDirectedGraph::removeEdge(Node* u, Node* v) {
    if(u == nullptr || v == nullptr) {
        return;
    }
    for(auto it = m_edges.begin(); it != m_edges.end();) {
        Edge* e = *it;
        if(e->startNode() == u && e->endNode() == v) {
            delete e;
            it = m_edges.erase(it);
        } else {
            ++it;
        }
    }
}
