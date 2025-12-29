#ifndef WDG_H
#define WDG_H

#include "Graph.h"

class WeightedDirectedGraph : public Graph {
public:
    WeightedDirectedGraph() = default;
    ~WeightedDirectedGraph() = default;

    void addEdge(const unsigned, const unsigned, const int w) override;
    void removeEdge(const unsigned) override;
    bool isDirected() const override;
};

#endif // WDG_H
