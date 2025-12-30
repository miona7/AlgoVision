#ifndef UUG_H
#define UUG_H

#include "Graph.h"

class UnweightedUndirectedGraph : public Graph {
public:
    UnweightedUndirectedGraph()  = default;
    ~UnweightedUndirectedGraph() override = default;

    void addEdge(unsigned, unsigned, int w = 1) override;
    void removeEdge(unsigned) override;
    bool isDirected() const override;
    bool isWeighted() const override;
};

#endif // UUG_H
