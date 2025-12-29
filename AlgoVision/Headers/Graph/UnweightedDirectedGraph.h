#ifndef UDG_H
#define UDG_H

#include "Graph.h"

class UnweightedDirectedGraph : public Graph {
public:
    UnweightedDirectedGraph() = default;
    ~UnweightedDirectedGraph() = default;

    void addEdge(const unsigned, const unsigned, const int w = 1) override;
    void removeEdge(const unsigned) override;
    bool isDirected() const override;
};

#endif // UDG_H
