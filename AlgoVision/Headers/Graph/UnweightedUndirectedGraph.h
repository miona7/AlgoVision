#ifndef UUG_H
#define UUG_H

#include "Graph.h"

class UnweightedUndirectedGraph : public Graph {
    void addEdge(Node*, Node*, int w = 1) override;
    void removeEdge(Node*, Node*) override;
};

#endif // UUG_H
