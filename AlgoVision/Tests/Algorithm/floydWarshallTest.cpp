#include <iostream>
#include <memory>

#include "FloydWarshall.h"
#include "WeightedDirectedGraph.h"

void noNegativeCycle() {

    std::shared_ptr<WeightedDirectedGraph> g = std::make_shared<WeightedDirectedGraph>();

    for(unsigned i = 1; i <= 4; ++i) {
        g->addNode(i);
    }

    g->addEdge(1, 2, 3);
    g->addEdge(2, 3, 2);
    g->addEdge(3, 4, 4);
    g->addEdge(1, 4, 10);

    FloydWarshall fw(g);

    try {
        std::cout << "Executing Floyd-Warshall on graph without negative cycle:" << std::endl;
        fw.execute();
    } catch(const std::runtime_error& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

void negativeCycle() {

    std::shared_ptr<WeightedDirectedGraph> g = std::make_shared<WeightedDirectedGraph>();

    for(unsigned i = 1; i <= 3; ++i) {
        g->addNode(i);
    }

    g->addEdge(1, 2, 1);
    g->addEdge(2, 3, -2);
    g->addEdge(3, 1, -2);

    FloydWarshall fw(g);

    try {
        std::cout << "Executing Floyd-Warshall on graph with negative cycle:" << std::endl;
        fw.execute();
    } catch(const std::runtime_error& e) {
        std::cout << "Expected error: " << e.what() << std::endl;
    }
}

int main() {
    noNegativeCycle();
    negativeCycle();
    return 0;
}
