#ifndef WDG_H
#define WDG_H

#include "Graph.h"

class WeightedDirectedGraph : public Graph {
public:
    WeightedDirectedGraph()           = default;
    ~WeightedDirectedGraph() override = default;

    void addEdge(unsigned, unsigned, int) override;
    void removeEdge(unsigned) override;
    bool isDirected() const override;
    bool isWeighted() const override;
};

#endif // WDG_H
