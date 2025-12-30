#ifndef WDG_H
#define WDG_H

#include "Graph.h"

class WeightedDirectedGraph : public Graph {
public:
    WeightedDirectedGraph()  = default;
    ~WeightedDirectedGraph() override = default;

    void addEdge(unsigned, unsigned, int w) override;
    void removeEdge(unsigned) override;
    bool isDirected() const override;
};

#endif // WDG_H
