#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "UnweightedDirectedGraph.h"
#include "UnweightedUndirectedGraph.h"
#include "WeightedDirectedGraph.h"
#include "WeightedUndirectedGraph.h"

TEST_CASE("UnweightedDirectedGraph - add/remove edge", "[Graph]") {
    UnweightedDirectedGraph g;

    g.addNode(1);
    g.addNode(2);

    REQUIRE(g.isDirected());
    REQUIRE_FALSE(g.isWeighted());

    g.addEdge(1, 2);

    auto edges = g.getEdges();
    REQUIRE(edges.size() == 1);

    auto adj = g.getAdjacencyList();
    REQUIRE(adj.count(1) == 1);
    REQUIRE(adj.at(1).size() == 1);
    REQUIRE(adj.count(2) == 1);
    REQUIRE(adj.at(2).empty());

    unsigned edgeId = edges.begin()->first;
    REQUIRE(adj.at(1).begin()->second == 2);

    g.removeEdge(edgeId);

    REQUIRE(g.getEdges().empty());

    adj = g.getAdjacencyList();
    REQUIRE(adj.at(1).empty());
}

TEST_CASE("UnweightedUndirectedGraph - add/remove edge", "[Graph]") {
    UnweightedUndirectedGraph g;

    g.addNode(1);
    g.addNode(2);

    REQUIRE_FALSE(g.isDirected());
    REQUIRE_FALSE(g.isWeighted());

    g.addEdge(1, 2);

    auto edges = g.getEdges();
    REQUIRE(edges.size() == 1);

    auto adj = g.getAdjacencyList();
    REQUIRE(adj.at(1).size() == 1);
    REQUIRE(adj.at(2).size() == 1);

    unsigned edgeId = edges.begin()->first;
    REQUIRE(adj.at(1).begin()->second == 2);
    REQUIRE(adj.at(2).begin()->second == 1);

    g.removeEdge(edgeId);

    REQUIRE(g.getEdges().empty());

    adj = g.getAdjacencyList();
    REQUIRE(adj.at(1).empty());
    REQUIRE(adj.at(2).empty());
}

TEST_CASE("WeightedDirectedGraph - add/remove edge", "[Graph]") {
    WeightedDirectedGraph g;

    g.addNode(1);
    g.addNode(2);

    REQUIRE(g.isDirected());
    REQUIRE(g.isWeighted());

    g.addEdge(1, 2, 7);

    auto edges = g.getEdges();
    REQUIRE(edges.size() == 1);

    const auto& e = edges.begin()->second;
    REQUIRE(e.startNode() == 1);
    REQUIRE(e.endNode() == 2);
    REQUIRE(e.getWeight() == 7);

    auto adj = g.getAdjacencyList();
    REQUIRE(adj.at(1).size() == 1);
    REQUIRE(adj.at(2).empty());

    unsigned edgeId = edges.begin()->first;
    g.removeEdge(edgeId);

    REQUIRE(g.getEdges().empty());
}

TEST_CASE("WeightedUndirectedGraph - add/remove edge", "[Graph]") {
    WeightedUndirectedGraph g;

    g.addNode(1);
    g.addNode(2);

    REQUIRE_FALSE(g.isDirected());
    REQUIRE(g.isWeighted());

    g.addEdge(1, 2, 10);

    auto edges = g.getEdges();
    REQUIRE(edges.size() == 1);

    const auto& e = edges.begin()->second;
    REQUIRE(e.getWeight() == 10);

    auto adj = g.getAdjacencyList();
    REQUIRE(adj.at(1).size() == 1);
    REQUIRE(adj.at(2).size() == 1);

    unsigned edgeId = edges.begin()->first;
    REQUIRE(adj.at(1).begin()->second == 2);
    REQUIRE(adj.at(2).begin()->second == 1);

    g.removeEdge(edgeId);

    REQUIRE(g.getEdges().empty());

    adj = g.getAdjacencyList();
    REQUIRE(adj.at(1).empty());
    REQUIRE(adj.at(2).empty());
}

TEST_CASE("Removing non-existing edge does nothing", "[Graph]") {
    UnweightedDirectedGraph g;

    g.addNode(1);
    g.addNode(2);

    g.removeEdge(999);

    REQUIRE(g.getEdges().empty());
}

TEST_CASE("UnweightedDirectedGraph remove isolated node", "[Graph]") {
    UnweightedDirectedGraph g;

    g.addNode(1);
    g.addNode(2);

    g.removeNode(1);

    auto nodes = g.getNodes();
    auto adj   = g.getAdjacencyList();

    REQUIRE(nodes.count(1) == 0);
    REQUIRE(nodes.size() == 1);
    REQUIRE(adj.count(1) == 0);
}

TEST_CASE("UnweightedDirectedGraph remove outgoing edges", "[Graph]") {
    UnweightedDirectedGraph g;

    g.addNode(1);
    g.addNode(2);
    g.addNode(3);

    g.addEdge(1, 2);
    g.addEdge(1, 3);

    REQUIRE(g.getEdges().size() == 2);

    g.removeNode(1);

    REQUIRE(g.getEdges().empty());
}

TEST_CASE("UnweightedDirectedGraph remove incoming edges", "[Graph]") {
    UnweightedDirectedGraph g;

    g.addNode(1);
    g.addNode(2);
    g.addNode(3);

    g.addEdge(2, 1);
    g.addEdge(3, 1);

    g.removeNode(1);

    REQUIRE(g.getEdges().empty());

    auto adj = g.getAdjacencyList();
    REQUIRE(adj.at(2).empty());
    REQUIRE(adj.at(3).empty());
}

TEST_CASE("UnweightedUndirectedGraph remove node", "[Graph]") {
    UnweightedUndirectedGraph g;

    g.addNode(1);
    g.addNode(2);
    g.addNode(3);

    g.addEdge(1, 2);
    g.addEdge(2, 3);

    REQUIRE(g.getEdges().size() == 2);

    g.removeNode(2);

    REQUIRE(g.getEdges().empty());

    auto adj = g.getAdjacencyList();
    REQUIRE(adj.at(1).empty());
    REQUIRE(adj.at(3).empty());
}

TEST_CASE("WeightedDirectedGraph remove node", "[Graph]") {
    WeightedDirectedGraph g;

    g.addNode(1);
    g.addNode(2);
    g.addNode(3);

    g.addEdge(1, 2, 5);
    g.addEdge(3, 1, 7);

    REQUIRE(g.getEdges().size() == 2);

    g.removeNode(1);

    REQUIRE(g.getEdges().empty());

    auto adj = g.getAdjacencyList();
    REQUIRE(adj.at(2).empty());
    REQUIRE(adj.at(3).empty());
}

TEST_CASE("WeightedUndirectedGraph remove node", "[Graph]") {
    WeightedUndirectedGraph g;

    g.addNode(1);
    g.addNode(2);
    g.addNode(3);

    g.addEdge(1, 2, 4);
    g.addEdge(2, 3, 6);

    REQUIRE(g.getEdges().size() == 2);

    g.removeNode(2);

    REQUIRE(g.getEdges().empty());

    auto adj = g.getAdjacencyList();
    REQUIRE(adj.at(1).empty());
    REQUIRE(adj.at(3).empty());
}

TEST_CASE("removeNode on non-existing node does nothing", "[Graph]") {
    UnweightedDirectedGraph g;

    g.addNode(1);
    g.addNode(2);

    g.removeNode(99);

    REQUIRE(g.getNodes().size() == 2);
    REQUIRE(g.getEdges().empty());
}
