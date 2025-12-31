#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "Kahn.h"
#include "UnweightedDirectedGraph.h"

TEST_CASE("Kahn topological sort on acyclic graph", "[KAHN]") {
    auto graph = std::make_shared<UnweightedDirectedGraph>();

    for(unsigned i = 1; i <= 5; ++i) {
        graph->addNode(i);
    }

    graph->addEdge(1, 2);
    graph->addEdge(1, 3);
    graph->addEdge(3, 4);
    graph->addEdge(2, 4);
    graph->addEdge(4, 5);

    Kahn kahn(graph);
    REQUIRE_NOTHROW(kahn.execute());

    auto sorted = kahn.getSorted();
    REQUIRE(sorted.size() == 5);

    auto pos = std::map<unsigned, unsigned>();
    for(unsigned i = 0; i < sorted.size(); ++i)
        pos[sorted[i]] = i;

    REQUIRE(pos[1] < pos[2]);
    REQUIRE(pos[1] < pos[3]);
    REQUIRE(pos[2] < pos[4]);
    REQUIRE(pos[3] < pos[4]);
    REQUIRE(pos[4] < pos[5]);
}

TEST_CASE("Kahn throws on cyclic graph", "[KAHN]") {
    auto graph = std::make_shared<UnweightedDirectedGraph>();

    graph->addNode(1);
    graph->addNode(2);
    graph->addNode(3);

    graph->addEdge(1, 2);
    graph->addEdge(2, 3);
    graph->addEdge(3, 1);

    Kahn kahn(graph);
    REQUIRE_THROWS_WITH(kahn.execute(), "Graph contains a cycle, topological sort not possible!");
}
