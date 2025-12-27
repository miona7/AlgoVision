#ifndef WUG_H
#define WUG_H

#include "Graph.h"

class WeightedUndirectedGraph : public Graph {
    void addEdge(Node*, Node*, int w = 1) override;
    void removeEdge(Node*, Node*) override;
};

#endif // WUG_H
