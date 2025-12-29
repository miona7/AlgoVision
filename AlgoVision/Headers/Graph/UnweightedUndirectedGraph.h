#ifndef UUG_H
#define UUG_H

#include "Graph.h"

class UnweightedUndirectedGraph : public Graph {
public:
    UnweightedUndirectedGraph() = default;
    ~UnweightedUndirectedGraph() = default;

    void addEdge(const unsigned, const unsigned, const int w = 1) override;
    void removeEdge(const unsigned) override;
};

#endif // UUG_H
