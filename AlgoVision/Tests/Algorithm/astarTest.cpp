#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "AStar.h"
#include "WeightedDirectedGraph.h"

TEST_CASE("AStar: simple path exists", "[AStar]") {
    auto g = std::make_shared<WeightedDirectedGraph>();
    for(unsigned i = 1; i <= 4; ++i) {
        g->addNode(i);
    }

    g->addEdge(1, 2, 1);
    g->addEdge(2, 3, 2);
    g->addEdge(3, 4, 3);
    g->addEdge(1, 4, 10);

    AStar astar(g);
    REQUIRE_NOTHROW(astar.execute(1, 4));

    std::vector<unsigned> expectedPath = {1, 2, 3, 4};
    REQUIRE(astar.getPath() == expectedPath);
    REQUIRE(astar.getTotalCost() == 6);
}

TEST_CASE("A* finds shortest path in simple graph", "[AStar]") {
    auto graph = std::make_shared<WeightedDirectedGraph>();

    graph->addNode(1, 0, 0);
    graph->addNode(2, 1, 0);
    graph->addNode(3, 1, 1);
    graph->addNode(4, 2, 1);

    graph->addEdge(1, 2, 1);
    graph->addEdge(2, 4, 2);
    graph->addEdge(1, 3, 2);
    graph->addEdge(3, 4, 1);

    AStar astar(graph);
    REQUIRE_NOTHROW(astar.execute(1, 4));

    std::vector<unsigned> expectedPath = {1, 2, 4}; // {1, 3, 4}
    REQUIRE(astar.getPath() == expectedPath);
    REQUIRE(astar.getTotalCost() == 3);
}

TEST_CASE("AStar: no path exists", "[AStar]") {
    auto g = std::make_shared<WeightedDirectedGraph>();
    g->addNode(1);
    g->addNode(2);

    AStar astar(g);
    REQUIRE_THROWS_WITH(astar.execute(1, 2), "No path found from start to goal!");
}

TEST_CASE("AStar: negative edge weight", "[AStar]") {
    auto g = std::make_shared<WeightedDirectedGraph>();
    g->addNode(1);
    g->addNode(2);
    g->addEdge(1, 2, -5);

    AStar astar(g);
    REQUIRE_THROWS_WITH(astar.execute(1, 2), "Graph contains negative edge weights!");
}
