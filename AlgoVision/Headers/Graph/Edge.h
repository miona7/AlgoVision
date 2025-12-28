#ifndef EDGE_H
#define EDGE_H

#include "Node.h"
#include <iostream>

class Edge {
public:
    Edge(Node*, Node*, bool directed = false, int weight = 1);
    Edge(const Edge&);
    ~Edge();

    bool  isDirected() const;
    int   getWeight() const;
    Node* startNode() const;
    Node* endNode() const;

    void setWeight(int);

    friend std::ostream& operator<<(std::ostream&, const Edge&);

private:
    bool                    m_directed;
    int                     m_weight;
    std::pair<Node*, Node*> m_connectedNodes;
};

std::ostream& operator<<(std::ostream&, const Edge&);

#endif // EDGE_H
