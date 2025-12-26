#ifndef GRAPH_H
#define GRAPH_H

#include <QVector>
#include <QSet>

#include "Node.h"
#include "Edge.h"

class Graph {
public:
    // Graph();
    Graph(bool directed = false, bool weighted = false);
    Graph(const Graph*);
    ~Graph();

    void addNode(Node*);
    void addEdge(Node*, Node*);
    void addEdge(Node*, Node*, int);

    void removeNode(Node*);
    void removeEdge(Node*, Node*);

    bool isDirected() const;
    bool isWeighted() const;

    int size() const;

private:
    bool m_directed;
    bool m_weighted;
    QVector<Node*> m_nodes;
    QVector<Edge*> m_edges;
    QSet<Node*> m_nodesIds;
};

#endif // GRAPH_H
