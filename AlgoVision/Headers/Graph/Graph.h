#ifndef GRAPH_H
#define GRAPH_H

#include <QString>
#include <QVariant>
#include <QVariantMap>
#include <QVariantList>

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

    std::vector<Node*> getNodesMutable();
    std::vector<Edge*> getEdgesMutable();

    Node*       getNode(unsigned);
    const Node* getNode(unsigned) const;

    Edge*       getEdge(unsigned);
    const Edge* getEdge(unsigned) const;
    Edge*       getEdge(unsigned, unsigned);
    const Edge* getEdge(unsigned, unsigned) const;

    void addEdgeSerialized(unsigned, unsigned, unsigned, int);

    QVariant toVariant() const override;
    void     fromVariant(const QVariant&) override;

    void clear();

protected:
    unsigned                                         m_nodeId     = 0;
    unsigned                                         m_edgeId     = 0;
    unsigned                                         m_numOfNodes = 0;
    unsigned                                         m_numOfEdges = 0;
    std::map<unsigned, std::map<unsigned, unsigned>> m_adjacencyList;
    std::map<unsigned, Node>                         m_nodes;
    std::map<unsigned, Edge>                         m_edges;
};

#endif // GRAPH_H
