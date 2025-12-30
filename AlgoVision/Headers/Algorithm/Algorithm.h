#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <memory>

#include "Edge.h"
#include "Graph.h"
#include "Node.h"

class Algorithm {
public:
    explicit Algorithm(const std::shared_ptr<Graph>& g);
    virtual ~Algorithm() = default;

    virtual bool checkConditions() const = 0;

    // razmisliti da bude protected
    virtual void execute(unsigned idStartNode = 0, unsigned idEndNode = 0) = 0;

    // getStates

protected:
    std::shared_ptr<Graph> m_graph;
    // states
};

#endif
