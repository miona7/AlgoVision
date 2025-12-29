#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <memory>

#include "Graph.h"
#include "Node.h"
#include "Edge.h"

class Algorithm {
public:
    explicit Algorithm(std::shared_ptr<Graph> g);
    virtual ~Algorithm() = default;

    virtual bool checkConditions() const = 0;

    // razmisliti da bude protected
    virtual void execute(const unsigned idStartNode = 0, const unsigned idEndNode = 0) = 0;

    // getStates

protected:
    std::shared_ptr<Graph> m_graph;
    // states
};

#endif
