#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "FloydWarshall.h"
#include "WeightedDirectedGraph.h"

TEST_CASE("Floyd-Warshall: graph without negative cycle", "[FW]") {
    auto graph = std::make_shared<WeightedDirectedGraph>();
    for(unsigned i = 1; i <= 4; ++i) {
        graph->addNode(i);
    }

    graph->addEdge(1, 2, 3);
    graph->addEdge(2, 3, 2);
    graph->addEdge(3, 4, 4);
    graph->addEdge(1, 4, 10);

    FloydWarshall fw(graph);

    REQUIRE_NOTHROW(fw.execute());
}

TEST_CASE("Floyd-Warshall: graph with negative cycle", "[FW]") {
    auto graph = std::make_shared<WeightedDirectedGraph>();
    for(unsigned i = 1; i <= 3; ++i) {
        graph->addNode(i);
    }

    graph->addEdge(1, 2, 1);
    graph->addEdge(2, 3, -2);
    graph->addEdge(3, 1, -2);

    FloydWarshall fw(graph);

    REQUIRE_THROWS_AS(fw.execute(), std::runtime_error);
}
