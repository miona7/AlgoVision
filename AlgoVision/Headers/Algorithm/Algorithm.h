#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "Edge.h"
#include "Graph.h"
#include "Node.h"

// #include <memory>

class Algorithm {
public:
    virtual ~Algorithm() = default;

    virtual bool checkConditions() const = 0;

    // razmisliti da bude protected
    virtual void execute(Node* startNode = nullptr, Node* endNode = nullptr) = 0;

    // getStates

protected:
    // std::unique_ptr<Graph> m_graph
    // std::shared_ptr<Graph> m_graph

    Graph* m_graph;
    // states
};

#endif
