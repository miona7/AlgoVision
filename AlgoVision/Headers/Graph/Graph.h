#ifndef GRAPH_H
#define GRAPH_H

#include <QVariant>
#include <QVariantList>
#include <QVariantMap>
#include <QVector>

#include <map>
#include <set>
#include <vector>

#include "Edge.h"
#include "Node.h"
#include "Serializable.h"

class Graph : public Serializable {
public:
    Graph()           = default;
    ~Graph() override = default;

    Node* addNode(double, double);
    void  addNode(unsigned, double = 0.0, double = 0.0);
    void  removeNode(unsigned);

    virtual void addEdge(unsigned, unsigned, int = 1) = 0;
    virtual void removeEdge(unsigned)                 = 0;

    virtual bool isDirected() const = 0;
    virtual bool isWeighted() const = 0;

    std::map<unsigned, std::map<unsigned, unsigned>> getAdjacencyList() const;
    std::map<unsigned, Node>                         getNodes() const;
    std::map<unsigned, Edge>                         getEdges() const;

    Node*       getNode(unsigned);
    const Node* getNode(unsigned) const;

    std::vector<Node*> getNodesMutable();

    Edge*       getEdge(unsigned);
    const Edge* getEdge(unsigned) const;
    Edge*       getEdge(unsigned, unsigned);
    const Edge* getEdge(unsigned, unsigned) const;

    std::vector<Edge*> getEdgesMutable();

    QVariant toVariant() const override;
    void     fromVariant(const QVariant&) override;

    void clear();

protected:
    unsigned                                         m_nodeId     = 0;
    unsigned                                         m_edgeId     = 0;
    unsigned                                         m_numOfNodes = 0;
    unsigned                                         m_numOfEdges = 0;
    std::map<unsigned, std::map<unsigned, unsigned>> m_adjacencyList;

    std::map<unsigned, Node> m_nodes;
    std::map<unsigned, Edge> m_edges;

    void addEdgeSerialized(unsigned, unsigned, unsigned, int);
};

#endif // GRAPH_H
