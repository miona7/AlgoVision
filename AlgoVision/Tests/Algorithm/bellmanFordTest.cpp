#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "BellmanFord.h"
#include "WeightedDirectedGraph.h"
#include "WeightedUndirectedGraph.h"

TEST_CASE("Bellman-Ford without negative cycle", "[BF]") {
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
    REQUIRE_NOTHROW(bf.execute(1));
    REQUIRE_FALSE(bf.hasNegativeCycle());
}

TEST_CASE("Bellman-Ford with negative cycle", "[BF]") {
    auto graph = std::make_shared<WeightedDirectedGraph>();

    for(unsigned i = 1; i <= 3; ++i) {
        graph->addNode(i);
    }

    graph->addEdge(1, 2, 1);
    graph->addEdge(2, 3, -2);
    graph->addEdge(3, 1, -2);

    BellmanFord bf(graph);
    REQUIRE_NOTHROW(bf.execute(1));
    REQUIRE(bf.hasNegativeCycle());
}

TEST_CASE("Bellman-Ford on invalid graph", "[BF]") {
    auto wud = std::make_shared<WeightedUndirectedGraph>();
    wud->addNode(1);
    wud->addNode(2);
    wud->addEdge(1, 2, 6);

    BellmanFord bf(wud);

    REQUIRE_THROWS_AS(bf.execute(1), std::runtime_error);
}

TEST_CASE("Bellman-Ford: start node does not exist", "[BF]") {
    auto graph = std::make_shared<WeightedDirectedGraph>();
    for(unsigned i = 1; i <= 2; ++i) {
        graph->addNode(i);
    }

    BellmanFord bf(graph);
    REQUIRE_THROWS_AS(bf.execute(0), std::runtime_error);
}

TEST_CASE("Bellman-Ford: empty graph", "[BF]") {
    auto graph = std::make_shared<WeightedDirectedGraph>();
    BellmanFord bf(graph);
    REQUIRE_THROWS_AS(bf.execute(0), std::runtime_error);
}

TEST_CASE("Bellman-Ford: graph with disconnected nodes", "[BF]") {
    auto graph = std::make_shared<WeightedDirectedGraph>();
    for(unsigned i = 1; i <= 4; ++i) {
        graph->addNode(i);
    }

    graph->addEdge(1, 2, 3);
    graph->addEdge(2, 3, 2);

    BellmanFord bf(graph);
    REQUIRE_NOTHROW(bf.execute(1));
}
