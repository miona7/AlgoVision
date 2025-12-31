#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "Tarjan.h"
#include "UnweightedDirectedGraph.h"
#include "WeightedDirectedGraph.h"
#include "UnweightedUndirectedGraph.h"

TEST_CASE("Tarjan on Unweighted Directed Graph", "[TARJAN]") {
    auto udg = std::make_shared<UnweightedDirectedGraph>();
    for(unsigned i = 1; i <= 5; ++i) {
        udg->addNode(i);
    }

    udg->addEdge(1, 2);
    udg->addEdge(2, 3);
    udg->addEdge(3, 1);
    udg->addEdge(3, 4);
    udg->addEdge(4, 5);

    Tarjan tarjan(udg);

    REQUIRE_NOTHROW(tarjan.execute());
}

TEST_CASE("Tarjan on Weighted Directed Graph", "[TARJAN]") {
    auto wdg = std::make_shared<WeightedDirectedGraph>();
    for(unsigned i = 1; i <= 6; ++i) {
        wdg->addNode(i);
    }

    wdg->addEdge(1, 2, 1);
    wdg->addEdge(2, 3, 2);
    wdg->addEdge(3, 1, 3);
    wdg->addEdge(3, 4, 1);
    wdg->addEdge(4, 5, 2);
    wdg->addEdge(5, 6, 1);
    wdg->addEdge(6, 4, 3);

    Tarjan tarjan(wdg);

    REQUIRE_NOTHROW(tarjan.execute());
}

TEST_CASE("Tarjan fails on Undirected Graph", "[TARJAN]") {
    auto uug = std::make_shared<UnweightedUndirectedGraph>();
    for(unsigned i = 1; i <= 4; ++i) {
        uug->addNode(i);
    }

    uug->addEdge(1, 2);
    uug->addEdge(2, 3);
    uug->addEdge(3, 4);

    Tarjan tarjan(uug);

    REQUIRE_THROWS_AS(tarjan.execute(), std::runtime_error);
}
