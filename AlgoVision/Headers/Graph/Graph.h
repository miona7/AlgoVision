#ifndef GRAPH_H
#define GRAPH_H

#include <QVector>
#include <QVariant>

#include <map>
#include <vector>

#include "Edge.h"
#include "Node.h"
#include "Serialization/Serializable.h"

class Graph : public Serializable{
public:
    Graph()          = default;
    virtual ~Graph() = default;

    void addNode(unsigned, double = 0.0, double = 0.0);
    void removeNode(unsigned);

    virtual void addEdge(unsigned, unsigned, int = 1) = 0;
    virtual void removeEdge(unsigned)                 = 0;

    virtual bool isDirected() const = 0;
    virtual bool isWeighted() const = 0;

    std::map<unsigned, std::map<unsigned, unsigned>> getAdjacencyList() const;
    std::map<unsigned, Node>                         getNodes() const;
    std::map<unsigned, Edge>                         getEdges() const;

    QVariant toVariant() const override;
    void fromVariant(const QVariant& variant) override;

protected:
    void clear();
    void addEdgeSerialized(unsigned edgeId, unsigned from, unsigned to, int w);

    unsigned                                         m_numOfNodes = 0;
    unsigned                                         m_numOfEdges = 0;
    std::map<unsigned, std::map<unsigned, unsigned>> m_adjacencyList;

    std::map<unsigned, Node> m_nodes;
    std::map<unsigned, Edge> m_edges;
};

#endif // GRAPH_H
