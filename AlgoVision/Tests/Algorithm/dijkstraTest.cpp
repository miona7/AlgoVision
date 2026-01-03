#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "Dijkstra.h"
#include "UnweightedDirectedGraph.h"
#include "UnweightedUndirectedGraph.h"
#include "WeightedDirectedGraph.h"
#include "WeightedUndirectedGraph.h"

TEST_CASE("Dijkstra on Unweighted Directed Graph", "[DIJKSTRA]") {
    auto udg = std::make_shared<UnweightedDirectedGraph>();
    for(unsigned i = 1; i <= 5; ++i) {
        udg->addNode(i);
    }

    udg->addEdge(1, 2);
    udg->addEdge(1, 3);
    udg->addEdge(2, 4);
    udg->addEdge(3, 4);
    udg->addEdge(4, 5);

    Dijkstra dijkstra(udg);
    REQUIRE_NOTHROW(dijkstra.execute(1));
}

TEST_CASE("Dijkstra on Unweighted Undirected Graph", "[DIJKSTRA]") {
    auto uug = std::make_shared<UnweightedUndirectedGraph>();
    for(unsigned i = 1; i <= 5; ++i) {
        uug->addNode(i);
    }

    uug->addEdge(1, 2);
    uug->addEdge(1, 3);
    uug->addEdge(2, 4);
    uug->addEdge(3, 4);
    uug->addEdge(4, 5);

    Dijkstra dijkstra(uug);
    REQUIRE_NOTHROW(dijkstra.execute(1));
}

TEST_CASE("Dijkstra on Weighted Directed Graph", "[DIJKSTRA]") {
    auto wdg = std::make_shared<WeightedDirectedGraph>();
    for(unsigned i = 1; i <= 6; ++i) {
        wdg->addNode(i);
    }

    wdg->addEdge(1, 2, 2);
    wdg->addEdge(1, 3, 5);
    wdg->addEdge(2, 4, 1);
    wdg->addEdge(3, 4, 2);
    wdg->addEdge(4, 5, 1);
    wdg->addEdge(5, 6, 3);

    Dijkstra dijkstra(wdg);
    REQUIRE_NOTHROW(dijkstra.execute(1));
}

TEST_CASE("Dijkstra on Weighted Undirected Graph", "[DIJKSTRA]") {
    auto wug = std::make_shared<WeightedUndirectedGraph>();
    for(unsigned i = 1; i <= 5; ++i) {
        wug->addNode(i);
    }

    wug->addEdge(1, 2, 2);
    wug->addEdge(1, 3, 4);
    wug->addEdge(2, 4, 1);
    wug->addEdge(3, 4, 3);
    wug->addEdge(4, 5, 5);

    Dijkstra dijkstra(wug);
    REQUIRE_NOTHROW(dijkstra.execute(1));
}

TEST_CASE("Dijkstra with invalid start node", "[DIJKSTRA]") {
    auto udg = std::make_shared<UnweightedDirectedGraph>();
    udg->addNode(1);
    udg->addNode(2);

    Dijkstra dijkstra(udg);
    REQUIRE_THROWS_AS(dijkstra.execute(0), std::runtime_error);
}

TEST_CASE("Dijkstra on graph with negative edges", "[DIJKSTRA]") {
    auto wdg = std::make_shared<WeightedDirectedGraph>();
    for(unsigned i = 1; i <= 3; ++i) {
        wdg->addNode(i);
    }

    wdg->addEdge(1, 2, -5);
    wdg->addEdge(2, 3, 2);

    Dijkstra dijkstra(wdg);
    REQUIRE_THROWS_AS(dijkstra.execute(1), std::runtime_error);
}
