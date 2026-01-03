#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "Prim.h"
#include "WeightedDirectedGraph.h"
#include "WeightedUndirectedGraph.h"

TEST_CASE("Prim works on connected undirected weighted graph", "[PRIM]") {
    auto g = std::make_shared<WeightedUndirectedGraph>();

    g->addNode(1);
    g->addNode(2);
    g->addNode(3);
    g->addNode(4);

    g->addEdge(1, 2, 1);
    g->addEdge(2, 3, 2);
    g->addEdge(3, 4, 3);
    g->addEdge(1, 4, 10);

    Prim prim(g);

    REQUIRE_NOTHROW(prim.execute());
}

TEST_CASE("Prim throws on disconnected graph", "[PRIM]") {
    auto g = std::make_shared<WeightedUndirectedGraph>();

    g->addNode(1);
    g->addNode(2);
    g->addNode(3);

    g->addEdge(1, 2, 1);
    // cvor 3 je izolovan

    Prim prim(g);

    REQUIRE_THROWS_AS(prim.execute(), std::runtime_error);
}

TEST_CASE("Prim throws on directed graph", "[PRIM]") {
    auto g = std::make_shared<WeightedDirectedGraph>();

    g->addNode(1);
    g->addNode(2);

    g->addEdge(1, 2, 5);

    Prim prim(g);

    REQUIRE_THROWS_AS(prim.execute(), std::runtime_error);
}

TEST_CASE("Prim throws on empty graph", "[PRIM]") {
    auto g = std::make_shared<WeightedUndirectedGraph>();

    Prim prim(g);

    REQUIRE_THROWS_AS(prim.execute(), std::runtime_error);
}
