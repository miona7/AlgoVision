#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "DFS.h"
#include "UnweightedDirectedGraph.h"
#include "UnweightedUndirectedGraph.h"
#include "WeightedDirectedGraph.h"
#include "WeightedUndirectedGraph.h"

TEST_CASE("DFS on Unweighted Directed Graph", "[DFS]") {
    auto graph = std::make_shared<UnweightedDirectedGraph>();

    for(unsigned i = 1; i <= 5; ++i) {
        graph->addNode(i);
    }

    graph->addEdge(1, 2);
    graph->addEdge(1, 3);
    graph->addEdge(2, 4);
    graph->addEdge(3, 5);

    DFS dfs(graph);
    REQUIRE_NOTHROW(dfs.execute(1));
}

TEST_CASE("DFS on Unweighted Undirected Graph", "[DFS]") {
    auto graph = std::make_shared<UnweightedUndirectedGraph>();

    for(unsigned i = 1; i <= 5; ++i) {
        graph->addNode(i);
    }

    graph->addEdge(1, 2);
    graph->addEdge(1, 3);
    graph->addEdge(2, 4);
    graph->addEdge(3, 5);

    DFS dfs(graph);
    REQUIRE_NOTHROW(dfs.execute(1));
}

TEST_CASE("DFS on Weighted Directed Graph", "[DFS]") {
    auto graph = std::make_shared<WeightedDirectedGraph>();

    for(unsigned i = 1; i <= 4; ++i) {
        graph->addNode(i);
    }

    graph->addEdge(1, 2, 10);
    graph->addEdge(1, 3, 5);
    graph->addEdge(2, 4, 1);
    graph->addEdge(3, 4, 2);

    DFS dfs(graph);
    REQUIRE_NOTHROW(dfs.execute(1));
}

TEST_CASE("DFS on Weighted Undirected Graph", "[DFS]") {
    auto graph = std::make_shared<WeightedUndirectedGraph>();

    for(unsigned i = 1; i <= 4; ++i) {
        graph->addNode(i);
    }

    graph->addEdge(1, 2, 3);
    graph->addEdge(1, 3, 7);
    graph->addEdge(2, 4, 1);
    graph->addEdge(3, 4, 2);

    DFS dfs(graph);
    REQUIRE_NOTHROW(dfs.execute(1));
}

TEST_CASE("DFS throws for invalid start node", "[DFS]") {
    auto graph = std::make_shared<UnweightedDirectedGraph>();
    graph->addNode(1);
    graph->addNode(2);

    DFS dfs(graph);
    REQUIRE_THROWS_AS(dfs.execute(0), std::runtime_error);
}
