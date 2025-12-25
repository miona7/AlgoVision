#ifndef UDG_H
#define UDG_H

#include "Graph.h"

class UnweightedDirectedGraph : public Graph {
    void addEdge(Node*, Node*, int w = 1) override;
    void removeEdge(Node*, Node*) override;
};

#endif // UDG_H
