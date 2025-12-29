#ifndef WUG_H
#define WUG_H

#include "Graph.h"

class WeightedUndirectedGraph : public Graph {
public:
    WeightedUndirectedGraph()  = default;
    ~WeightedUndirectedGraph() = default;

    void addEdge(const unsigned, const unsigned, const int w) override;
    void removeEdge(const unsigned) override;
    bool isDirected() const override;
};

#endif // WUG_H
