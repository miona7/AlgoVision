#ifndef WUG_H
#define WUG_H

#include "Graph.h"

class WeightedUndirectedGraph : public Graph {
public:
    WeightedUndirectedGraph()  = default;
    ~WeightedUndirectedGraph() override = default;

    void addEdge(unsigned, unsigned, int w) override;
    void removeEdge(unsigned) override;
    bool isDirected() const override;
};

#endif // WUG_H
