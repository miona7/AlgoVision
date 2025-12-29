#include <iostream>
#include <memory>

#include "Dijkstra.h"
#include "UnweightedDirectedGraph.h"
#include "UnweightedUndirectedGraph.h"
#include "WeightedDirectedGraph.h"
#include "WeightedUndirectedGraph.h"

void testUDG() {

    auto udg = std::make_shared<UnweightedDirectedGraph>();
    for(unsigned i = 1; i <= 5; ++i)
        udg->addNode(i);
    udg->addEdge(1, 2);
    udg->addEdge(1, 3);
    udg->addEdge(2, 4);
    udg->addEdge(3, 4);
    udg->addEdge(4, 5);

    std::cout << "UDG" << std::endl;
    Dijkstra dijkstra(udg);
    try {
        dijkstra.execute(1);
    } catch(const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    std::cout << "------------------------------------------" << std::endl;
}

void testUUG() {
    auto uug = std::make_shared<UnweightedUndirectedGraph>();
    for(unsigned i = 1; i <= 5; ++i)
        uug->addNode(i);
    uug->addEdge(1, 2);
    uug->addEdge(1, 3);
    uug->addEdge(2, 4);
    uug->addEdge(3, 4);
    uug->addEdge(4, 5);

    std::cout << "UUG" << std::endl;
    Dijkstra dijkstra(uug);
    try {
        dijkstra.execute(1);
    } catch(const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    std::cout << "------------------------------------------" << std::endl;
}

void testWDG() {
    auto wdg = std::make_shared<WeightedDirectedGraph>();
    for(unsigned i = 1; i <= 6; ++i)
        wdg->addNode(i);
    wdg->addEdge(1, 2, 2);
    wdg->addEdge(1, 3, 5);
    wdg->addEdge(2, 4, 1);
    wdg->addEdge(3, 4, 2);
    wdg->addEdge(4, 5, 1);
    wdg->addEdge(5, 6, 3);

    std::cout << "WDG" << std::endl;
    Dijkstra dijkstra(wdg);
    try {
        dijkstra.execute(1);
    } catch(const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    std::cout << "------------------------------------------" << std::endl;
}

void testWUG() {
    auto wug = std::make_shared<WeightedUndirectedGraph>();
    for(unsigned i = 1; i <= 5; ++i)
        wug->addNode(i);
    wug->addEdge(1, 2, 2);
    wug->addEdge(1, 3, 4);
    wug->addEdge(2, 4, 1);
    wug->addEdge(3, 4, 3);
    wug->addEdge(4, 5, 5);

    std::cout << "WUG" << std::endl;
    Dijkstra dijkstra(wug);
    try {
        dijkstra.execute(1);
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
