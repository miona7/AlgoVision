#ifndef GRAPH_H
#define GRAPH_H

#include <QVector>

#include <map>
#include <vector>

#include "Edge.h"
#include "Node.h"

class Graph {
public:
    Graph()          = default;
    virtual ~Graph() = default;

    void addNode(const unsigned);
    void removeNode(const unsigned);

    virtual void addEdge(const unsigned, const unsigned, const int w = 1) = 0;
    virtual void removeEdge(const unsigned)                               = 0;

    virtual bool isDirected() const = 0;

    std::map<unsigned, std::map<unsigned, unsigned>> getAdjacencyList() const;
    std::map<unsigned, Node>                         getNodes() const;
    std::map<unsigned, Edge>                         getEdges() const;

protected:
    unsigned                                         m_numOfNodes = 0;
    unsigned                                         m_numOfEdges = 0;
    std::map<unsigned, std::map<unsigned, unsigned>> m_adjacencyList;

    std::map<unsigned, Node> m_nodes;
    std::map<unsigned, Edge> m_edges;
};

#endif // GRAPH_H
