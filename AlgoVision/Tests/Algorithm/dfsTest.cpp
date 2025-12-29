#include <iostream>
#include <memory>

#include "DFS.h"
#include "UnweightedDirectedGraph.h"
#include "UnweightedUndirectedGraph.h"
#include "WeightedDirectedGraph.h"
#include "WeightedUndirectedGraph.h"

void testUDG() {
    auto graph = std::make_shared<UnweightedDirectedGraph>();

    for(unsigned i = 1; i <= 5; ++i)
        graph->addNode(i);

    graph->addEdge(1, 2);
    graph->addEdge(1, 3);
    graph->addEdge(2, 4);
    graph->addEdge(3, 5);

    std::cout << "UDG" << std::endl;
    DFS dfs(graph);
    try {
        dfs.execute(1);
    } catch(const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    std::cout << "------------------------------------------" << std::endl;
}

void testUUG() {
    auto graph = std::make_shared<UnweightedUndirectedGraph>();

    for(unsigned i = 1; i <= 5; ++i)
        graph->addNode(i);

    graph->addEdge(1, 2);
    graph->addEdge(1, 3);
    graph->addEdge(2, 4);
    graph->addEdge(3, 5);

    std::cout << "UUG" << std::endl;
    DFS dfs(graph);
    try {
        dfs.execute(1);
    } catch(const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    std::cout << "------------------------------------------" << std::endl;
}

void testWDG() {
    auto graph = std::make_shared<WeightedDirectedGraph>();

    for(unsigned i = 1; i <= 4; ++i)
        graph->addNode(i);

    graph->addEdge(1, 2, 10);
    graph->addEdge(1, 3, 5);
    graph->addEdge(2, 4, 1);
    graph->addEdge(3, 4, 2);

    std::cout << "WDG" << std::endl;
    DFS dfs(graph);
    try {
        dfs.execute(1);
    } catch(const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    std::cout << "------------------------------------------" << std::endl;
}

void testWUG() {
    auto graph = std::make_shared<WeightedUndirectedGraph>();

    for(unsigned i = 1; i <= 4; ++i)
        graph->addNode(i);

    graph->addEdge(1, 2, 3);
    graph->addEdge(1, 3, 7);
    graph->addEdge(2, 4, 1);
    graph->addEdge(3, 4, 2);

    std::cout << "WUG" << std::endl;
    DFS dfs(graph);
    try {
        dfs.execute(1);
    } catch(const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    std::cout << "------------------------------------------" << std::endl;
}

int main() {

    testUDG();
    testUUG();
    testWDG();
    testWUG();

    return 0;
}
