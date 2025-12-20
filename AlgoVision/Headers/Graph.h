#ifndef GRAPH_H
#define GRAPH_H

#include <QSet>
#include <QVector>

#include "Edge.h"
#include "Node.h"

class Graph {
public:
    // Graph();
    Graph(bool directed = false, bool weighted = false);
    Graph(const Graph *);
    ~Graph();

    void addNode(Node *);
    void addEdge(Node *, Node *);
    void addEdge(Node *, Node *, bool d = false, int w = 1);

    void removeNode(Node *);
    void removeEdge(Node *, Node *);

    bool isDirected() const;
    bool isWeighted() const;

    QVector<Node*> getNodes() const;
    QVector<Edge*> getEdges() const;

private:
    bool m_directed;
    bool m_weighted;
    QVector<Node *> m_nodes;
    QVector<Edge *> m_edges;
    QSet<Node *> m_nodesIds;
};

#endif // GRAPH_H
