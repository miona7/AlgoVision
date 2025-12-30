#include <iostream>
#include <memory>

#include "BellmanFord.h"
#include "WeightedDirectedGraph.h"
#include "WeightedUndirectedGraph.h"

void noNegativeCycle() {
    std::cout << "Test: Bellman-Ford without negative cycle" << std::endl;
    auto graph = std::make_shared<WeightedDirectedGraph>();

    for(unsigned i = 1; i <= 5; ++i) {
        graph->addNode(i);
    }

    graph->addEdge(1, 2, 6);
    graph->addEdge(1, 3, 7);
    graph->addEdge(2, 4, 5);
    graph->addEdge(3, 4, -3);
    graph->addEdge(4, 5, 2);

    BellmanFord bf(graph);
    bf.execute(1);

    if(bf.hasNegativeCycle()) {
        std::cout << "Unexpected negative cycle detected!" << std::endl;
    }

    std::cout << "------------------------------------------" << std::endl;
}

void negativeCycle() {
    std::cout << "Test: Bellman-Ford with negative cycle" << std::endl;
    auto graph = std::make_shared<WeightedDirectedGraph>();

    for(unsigned i = 1; i <= 3; ++i) {
        graph->addNode(i);
    }

    graph->addEdge(1, 2, 1);
    graph->addEdge(2, 3, -2);
    graph->addEdge(3, 1, -2);

    BellmanFord bf(graph);
    bf.execute(1);

    if(bf.hasNegativeCycle()) {
        std::cout << "Negative cycle correctly detected!" << std::endl;
    } else {
        std::cout << "Failed to detect negative cycle!" << std::endl;
    }

    std::cout << "------------------------------------------" << std::endl;
}


void invalidGraph() {
    std::cout << "Test: Bellman-Ford on invalid graphs" << std::endl;

    auto wud = std::make_shared<WeightedUndirectedGraph>();
    wud->addNode(1);
    wud->addNode(2);
    wud->addEdge(1, 2, 6);

    try {
        BellmanFord bf(wud);
        bf.execute(1);
        std::cout << "Error: Bellman-Ford should not run on unweighted graph!" << std::endl;
    } catch(const std::runtime_error& e) {
        std::cout << "Correctly caught exception: " << e.what() << std::endl;
    }
}

int main() {
    noNegativeCycle();
    negativeCycle();
    invalidGraph();
    return 0;
}
