#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "Edge.h"
#include "Graph.h"
#include "Node.h"

class Algorithm {
public:
    virtual ~Algorithm();

    virtual bool checkConditions() const = 0;

    // razmisliti da bude protected
    virtual void execute(Node* startNode = nullptr, Node* endNode = nullptr) = 0;

    // getStates

protected:
    Graph* m_graph;
    // states
};

#endif
