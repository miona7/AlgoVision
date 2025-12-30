#ifndef UDG_H
#define UDG_H

#include "Graph.h"

class UnweightedDirectedGraph : public Graph {
public:
    UnweightedDirectedGraph()  = default;
    ~UnweightedDirectedGraph() override = default;

    void addEdge(unsigned, unsigned, int w = 1) override;
    void removeEdge(unsigned) override;
    bool isDirected() const override;
};

#endif // UDG_H
