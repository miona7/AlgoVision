#ifndef WDG_H
#define WDG_H

#include "Graph.h"

class WeightedDirectedGraph : public Graph {
    void addEdge(Node*, Node*, int w = 1) override;
    void removeEdge(Node*, Node*) override;
};

#endif // WDG_H
