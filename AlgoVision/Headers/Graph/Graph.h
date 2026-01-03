#ifndef GRAPH_H
#define GRAPH_H

#include <QSet>
#include <QVector>

#include "Edge.h"
#include "Node.h"

class Graph {
public:
    Graph();
    Graph(const Graph&);
    virtual ~Graph();

    void addNode(Node*);
    void removeNode(Node*);

    virtual void addEdge(Node*, Node*, int w = 1) = 0;
    virtual void removeEdge(Node*, Node*)         = 0;

    QVector<Node*> getNodes() const;
    QVector<Edge*> getEdges() const;

    void setNodes(QVector<Node*>&);
    void setEdges(QVector<Edge*>&);

    void clear();

protected:
    QVector<Node*> m_nodes;
    QVector<Edge*> m_edges;
};

#endif // GRAPH_H
