#include "Graph.h"

Graph::Graph() = default;

Graph::Graph(const Graph& g) {
    for(auto* n: g.m_nodes) {
        m_nodes.push_back(new Node(*n));
    }

    for(auto* e: g.m_edges) {
        m_edges.push_back(new Edge(*e));
    }
}

Graph::~Graph() {
    for(auto* n: m_nodes) {
        delete n;
    }

    for(auto* e: m_edges) {
        delete e;
    }
}

void Graph::addNode(Node *n) {
    m_nodes.append(n);
}

void Graph::removeNode(Node *n) {
    if(n == nullptr) {
        return;
    }

    // brise sve grane
    for(auto it = m_edges.begin(); it != m_edges.end();) {
        Edge* e = *it;
        if(e->startNode() == n || e->endNode() == n) {
            delete e;
            it = m_edges.erase(it);
        } else {
            ++it;
        }
    }

    // brise cvor
    for(auto it = m_nodes.begin(); it != m_nodes.end();) {
        if(*it == n) {
            delete *it;
            it = m_nodes.erase(it);
        } else {
            ++it;
        }
    }
}

QVector<Node*> Graph::getNodes() const {
    return m_nodes;
}

QVector<Edge*> Graph::getEdges() const {
    return m_edges;
}

void Graph::setNodes(QVector<Node*>& nodes) {
    clear();
    for(auto* n : nodes) {
        m_nodes.push_back(new Node(*n));
    }
}

void Graph::setEdges(QVector<Edge*>& edges) {
    clear();
    for(auto* e : edges) {
        m_edges.push_back(new Edge(*e));
    }
}

void Graph::clear() {
    for(auto* n: m_nodes) {
        delete n;
    }

    for(auto* e: m_edges) {
        delete e;
    }

    m_nodes.clear();
    m_edges.clear();
}

