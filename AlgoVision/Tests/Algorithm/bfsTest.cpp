#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "BFS.h"
#include "UnweightedDirectedGraph.h"
#include "UnweightedUndirectedGraph.h"
#include "WeightedDirectedGraph.h"
#include "WeightedUndirectedGraph.h"

TEST_CASE("BFS on Unweighted Directed Graph", "[BFS]") {
    auto g = std::make_shared<UnweightedDirectedGraph>();
    g->addNode(0);
    g->addNode(1);
    g->addNode(2);

    g->addEdge(0, 1);
    g->addEdge(1, 2);

    BFS bfs(g);
    REQUIRE_NOTHROW(bfs.execute(0));
}


TEST_CASE("BFS on Unweighted Undirected Graph", "[BFS]") {
    auto g = std::make_shared<UnweightedUndirectedGraph>();
    g->addNode(0);
    g->addNode(1);
    g->addNode(2);

    g->addEdge(0, 1);
    g->addEdge(1, 2);

    BFS bfs(g);
    REQUIRE_NOTHROW(bfs.execute(0));
}

TEST_CASE("BFS on Weighted Directed Graph", "[BFS]") {
    auto g = std::make_shared<WeightedDirectedGraph>();
    g->addNode(0);
    g->addNode(1);
    g->addNode(2);

    g->addEdge(0, 1, 5);
    g->addEdge(1, 2, 3);

    BFS bfs(g);
    REQUIRE_NOTHROW(bfs.execute(0));
}

TEST_CASE("BFS on Weighted Undirected Graph", "[BFS]") {
    auto g = std::make_shared<WeightedUndirectedGraph>();
    g->addNode(0);
    g->addNode(1);
    g->addNode(2);

    g->addEdge(0, 1, 2);
    g->addEdge(1, 2, 4);

    BFS bfs(g);
    REQUIRE_NOTHROW(bfs.execute(0));
}

TEST_CASE("BFS with invalid start node", "[BFS]") {
    auto g = std::make_shared<UnweightedDirectedGraph>();
    g->addNode(1);
    g->addNode(2);

    BFS bfs(g);
    REQUIRE_THROWS_AS(bfs.execute(0), std::runtime_error);
}
