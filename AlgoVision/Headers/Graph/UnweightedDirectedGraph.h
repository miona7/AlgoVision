#ifndef UDG_H
#define UDG_H

#include "Graph.h"

class UnweightedDirectedGraph : public Graph {
public:
    UnweightedDirectedGraph()           = default;
    ~UnweightedDirectedGraph() override = default;

    void addEdge(unsigned, unsigned, int = 1) override;
    void removeEdge(unsigned) override;
    bool isDirected() const override;
    bool isWeighted() const override;
};

#endif // UDG_H
