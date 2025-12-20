#include "Graph.h"

// Graph::Graph() {}

Graph::Graph(bool directed, bool weighted)
    : m_directed(directed), m_weighted(weighted) {
}

Graph::Graph(const Graph *g) : Graph(g->isDirected(), g->isWeighted()) {
}

Graph::~Graph() {
    for(auto n: m_nodes) {
        delete n;
    }

    for(auto e: m_edges) {
        delete e;
    }
}

void Graph::addNode(Node *n) {
    m_nodes.append(n);
}

void Graph::addEdge(Node *u, Node *v) {
    m_edges.append(new Edge(u, v));
}

void Graph::addEdge(Node *u, Node *v, bool d, int w) {
    m_edges.append(new Edge(u, v, d, w));
}

void Graph::removeNode(Node *n) {
    if(n == nullptr) {
        return;
    }
    for(auto it = m_nodes.begin(); it != m_nodes.end();) {
        if(*it == n) {
            it = m_nodes.erase(it);
        } else {
            ++it;
        }
    }
}

// void Graph::removeEdge(Node* u, Node* v) {}

bool Graph::isDirected() const {
    return m_directed;
}

bool Graph::isWeighted() const {
    return m_weighted;
}

QVector<Node*> Graph::getNodes() const {
    return m_nodes;
}

QVector<Edge*> Graph::getEdges() const {
    return m_edges;
}
