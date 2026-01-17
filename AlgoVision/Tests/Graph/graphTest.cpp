#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "Edge.h"
#include "Node.h"
#include "UnweightedDirectedGraph.h"
#include "UnweightedUndirectedGraph.h"
#include "WeightedDirectedGraph.h"
#include "WeightedUndirectedGraph.h"

TEST_CASE("UnweightedDirectedGraph - add/remove edge", "[Graph]") {
    // arrange
    UnweightedDirectedGraph g;

    // act
    g.addNode(1);
    g.addNode(2);
    g.addNode(1);

    // assert
    REQUIRE(g.isDirected());
    REQUIRE_FALSE(g.isWeighted());
    REQUIRE(g.getNodes().size() == 2);

    // act
    g.addEdge(1, 2);

    // assert
    auto edges = g.getEdges();
    auto adj   = g.getAdjacencyList();

    REQUIRE(edges.size() == 1);
    REQUIRE(adj.at(1).size() == 1);
    REQUIRE(adj.at(1).begin()->second == 2);
    REQUIRE(adj.at(2).empty());

    // act
    unsigned edgeId = edges.begin()->first;
    g.removeEdge(edgeId);
    adj = g.getAdjacencyList();

    // assert
    REQUIRE(g.getEdges().empty());
    REQUIRE(adj.at(1).empty());
}

TEST_CASE("UnweightedUndirectedGraph - add/remove edge", "[Graph]") {
    // arrange
    UnweightedUndirectedGraph g;

    // act
    g.addNode(1);
    g.addNode(2);
    g.addNode(1);

    // assert
    REQUIRE_FALSE(g.isDirected());
    REQUIRE_FALSE(g.isWeighted());
    REQUIRE(g.getNodes().size() == 2);

    // act
    g.addEdge(1, 2);
    auto edges = g.getEdges();
    auto adj   = g.getAdjacencyList();

    // assert
    REQUIRE(edges.size() == 1);
    REQUIRE(adj.at(1).size() == 1);
    REQUIRE(adj.at(2).size() == 1);
    REQUIRE(adj.at(1).begin()->second == 2);
    REQUIRE(adj.at(2).begin()->second == 1);

    // act
    unsigned edgeId = edges.begin()->first;
    g.removeEdge(edgeId);
    adj = g.getAdjacencyList();

    // assert
    REQUIRE(g.getEdges().empty());
    REQUIRE(adj.at(1).empty());
    REQUIRE(adj.at(2).empty());
}

TEST_CASE("WeightedDirectedGraph - add/remove edge", "[Graph]") {
    // arrange
    WeightedDirectedGraph g;

    // act
    g.addNode(1);
    g.addNode(2);
    g.addNode(1);

    // assert
    REQUIRE(g.isDirected());
    REQUIRE(g.isWeighted());
    REQUIRE(g.getNodes().size() == 2);

    // act
    g.addEdge(1, 2, 7);
    auto        edges = g.getEdges();
    const auto& e     = edges.begin()->second;
    auto        adj   = g.getAdjacencyList();

    // assert
    REQUIRE(edges.size() == 1);
    REQUIRE(e.startNode() == 1);
    REQUIRE(e.endNode() == 2);
    REQUIRE(e.getWeight() == 7);
    REQUIRE(adj.at(1).size() == 1);
    REQUIRE(adj.at(2).empty());

    // act
    unsigned edgeId = edges.begin()->first;
    g.removeEdge(edgeId);

    // assert
    REQUIRE(g.getEdges().empty());
}

TEST_CASE("WeightedUndirectedGraph - add/remove edge", "[Graph]") {
    // arrange
    WeightedUndirectedGraph g;

    // act
    g.addNode(1);
    g.addNode(2);
    g.addNode(1);

    // assert
    REQUIRE_FALSE(g.isDirected());
    REQUIRE(g.isWeighted());
    REQUIRE(g.getNodes().size() == 2);

    // act
    g.addEdge(1, 2, 10);
    auto        edges  = g.getEdges();
    const auto& e      = edges.begin()->second;
    auto        adj    = g.getAdjacencyList();
    unsigned    edgeId = edges.begin()->first;

    // assert
    REQUIRE(edges.size() == 1);
    REQUIRE(e.getWeight() == 10);
    REQUIRE(adj.at(1).size() == 1);
    REQUIRE(adj.at(2).size() == 1);
    REQUIRE(adj.at(1).begin()->second == 2);
    REQUIRE(adj.at(2).begin()->second == 1);

    // act
    g.removeEdge(edgeId);
    adj = g.getAdjacencyList();

    // assert
    REQUIRE(g.getEdges().empty());
    REQUIRE(adj.at(1).empty());
    REQUIRE(adj.at(2).empty());
}

TEST_CASE("Removing non-existing edge does nothing", "[Graph]") {
    // arrange
    UnweightedDirectedGraph g;

    // act
    g.addNode(1);
    g.addNode(2);
    g.removeEdge(999);

    // assert
    REQUIRE(g.getEdges().empty());
}

TEST_CASE("UnweightedDirectedGraph remove isolated node", "[Graph]") {
    // arrange
    UnweightedDirectedGraph g;

    // act
    g.addNode(1);
    g.addNode(2);
    g.removeNode(1);

    auto nodes = g.getNodes();
    auto adj   = g.getAdjacencyList();

    // assert
    REQUIRE(nodes.count(1) == 0);
    REQUIRE(nodes.size() == 1);
    REQUIRE(adj.count(1) == 0);
}

TEST_CASE("UnweightedDirectedGraph remove outgoing edges", "[Graph]") {
    // arrange
    UnweightedDirectedGraph g;

    // act
    g.addNode(1);
    g.addNode(2);
    g.addNode(3);
    g.addEdge(1, 2);
    g.addEdge(1, 3);

    // assert
    REQUIRE(g.getEdges().size() == 2);

    // act
    g.removeNode(1);

    // assert
    REQUIRE(g.getEdges().empty());
}

TEST_CASE("UnweightedDirectedGraph remove incoming edges", "[Graph]") {
    // arrange
    UnweightedDirectedGraph g;

    // act
    g.addNode(1);
    g.addNode(2);
    g.addNode(3);

    g.addEdge(2, 1);
    g.addEdge(3, 1);

    g.removeNode(1);
    auto adj = g.getAdjacencyList();

    // assert
    REQUIRE(g.getEdges().empty());
    REQUIRE(adj.at(2).empty());
    REQUIRE(adj.at(3).empty());
}

TEST_CASE("UnweightedUndirectedGraph remove node", "[Graph]") {
    // arrange
    UnweightedUndirectedGraph g;

    // act
    g.addNode(1);
    g.addNode(2);
    g.addNode(3);
    g.addEdge(1, 2);
    g.addEdge(2, 3);

    // assert
    REQUIRE(g.getEdges().size() == 2);

    // act
    g.removeNode(2);
    auto adj = g.getAdjacencyList();

    // assert
    REQUIRE(g.getEdges().empty());
    REQUIRE(adj.at(1).empty());
    REQUIRE(adj.at(3).empty());
}

TEST_CASE("WeightedDirectedGraph remove node", "[Graph]") {
    // arrange
    WeightedDirectedGraph g;

    // act
    g.addNode(1);
    g.addNode(2);
    g.addNode(3);
    g.addEdge(1, 2, 5);
    g.addEdge(3, 1, 7);

    // assert
    REQUIRE(g.getEdges().size() == 2);

    // act
    g.removeNode(1);
    auto adj = g.getAdjacencyList();

    // assert
    REQUIRE(g.getEdges().empty());
    REQUIRE(adj.at(2).empty());
    REQUIRE(adj.at(3).empty());
}

TEST_CASE("WeightedUndirectedGraph remove node", "[Graph]") {
    // arrange
    WeightedUndirectedGraph g;

    // act
    g.addNode(1);
    g.addNode(2);
    g.addNode(3);
    g.addEdge(1, 2, 4);
    g.addEdge(2, 3, 6);

    // assert
    REQUIRE(g.getEdges().size() == 2);

    // act
    g.removeNode(2);
    auto adj = g.getAdjacencyList();

    // assert
    REQUIRE(g.getEdges().empty());
    REQUIRE(adj.at(1).empty());
    REQUIRE(adj.at(3).empty());
}

TEST_CASE("removeNode on non-existing node does nothing", "[Graph]") {
    // arrange
    UnweightedDirectedGraph g;

    // act
    g.addNode(1);
    g.addNode(2);
    g.removeNode(99);

    // assert
    REQUIRE(g.getNodes().size() == 2);
    REQUIRE(g.getEdges().empty());
}

TEST_CASE("Testing getNode and getEdge", "[Graph]") {

    SECTION("Unweighted Directed Graph") {
        // arrange
        UnweightedDirectedGraph g;

        g.addNode(1);
        g.addNode(2);

        // act
        auto* node1 = g.getNode(1);
        auto* node2 = g.getNode(999);

        auto* edge1 = g.getEdge(1, 2);

        // assert
        REQUIRE_FALSE(node1 == nullptr);
        REQUIRE(node2 == nullptr);
        REQUIRE(edge1 == nullptr);

        // act
        g.addEdge(1, 2);
        edge1       = g.getEdge(1, 2);
        auto* edge2 = g.getEdge(2, 1);

        // assert
        REQUIRE_FALSE(edge1 == nullptr);
        REQUIRE(edge2 == nullptr);
    }

    SECTION("Unweighted Undirected Graph") {
        // arrange
        UnweightedUndirectedGraph g;
        g.addNode(1);
        g.addNode(2);

        // act
        auto* node1 = g.getNode(1);
        auto* node2 = g.getNode(999);
        auto* edge1 = g.getEdge(1, 2);

        // assert
        REQUIRE_FALSE(node1 == nullptr);
        REQUIRE(node2 == nullptr);
        REQUIRE(edge1 == nullptr);

        // act
        g.addEdge(1, 2);
        edge1       = g.getEdge(1, 2);
        auto* edge2 = g.getEdge(2, 1);

        // assert
        REQUIRE_FALSE(edge1 == nullptr);
        REQUIRE_FALSE(edge2 == nullptr);
    }

    SECTION("Weighted Directed Graph") {
        // arrange
        WeightedDirectedGraph g;
        g.addNode(1);
        g.addNode(2);

        // act
        auto* node1 = g.getNode(1);
        auto* node2 = g.getNode(999);
        auto* edge1 = g.getEdge(1, 2);

        // assert
        REQUIRE_FALSE(node1 == nullptr);
        REQUIRE(node2 == nullptr);
        REQUIRE(edge1 == nullptr);

        // act
        g.addEdge(1, 2, 5);
        edge1       = g.getEdge(1, 2);
        auto* edge2 = g.getEdge(2, 1);

        // assert
        REQUIRE_FALSE(edge1 == nullptr);
        REQUIRE(edge1->getWeight() == 5);
        REQUIRE(edge2 == nullptr);
    }

    SECTION("Weighted Undirected Graph") {
        // arrange
        WeightedUndirectedGraph g;
        g.addNode(1);
        g.addNode(2);

        // act
        auto* node1 = g.getNode(1);
        auto* node2 = g.getNode(999);
        auto* edge1 = g.getEdge(1, 2);

        // assert
        REQUIRE_FALSE(node1 == nullptr);
        REQUIRE(node2 == nullptr);
        REQUIRE(edge1 == nullptr);

        // act
        g.addEdge(1, 2, 10);
        edge1       = g.getEdge(1, 2);
        auto* edge2 = g.getEdge(2, 1);

        // assert
        REQUIRE_FALSE(edge1 == nullptr);
        REQUIRE(edge1->getWeight() == 10);
        REQUIRE_FALSE(edge2 == nullptr);
    }
}

TEST_CASE("Testing const getNode and getEdge coverage", "[Graph]") {

    SECTION("Unweighted Directed Graph") {
        // arrange
        UnweightedDirectedGraph g;
        g.addNode(1);
        g.addNode(2);
        g.addEdge(1, 2);
        const auto& cg = g;

        // act
        const Node* node1 = cg.getNode(1);
        const Node* node2 = cg.getNode(999);
        const Edge* edge1 = cg.getEdge(1, 2);
        const Edge* edge2 = cg.getEdge(2, 1);

        // assert
        REQUIRE_FALSE(node1 == nullptr);
        REQUIRE(node2 == nullptr);
        REQUIRE_FALSE(edge1 == nullptr);
        REQUIRE(edge2 == nullptr); // directed
    }

    SECTION("Unweighted Undirected Graph") {
        // arrange
        UnweightedUndirectedGraph g;
        g.addNode(1);
        g.addNode(2);
        g.addEdge(1, 2);
        const auto& cg = g;

        // act
        const Node* node1 = cg.getNode(1);
        const Node* node2 = cg.getNode(999);
        const Edge* edge1 = cg.getEdge(1, 2);
        const Edge* edge2 = cg.getEdge(2, 1);

        // assert
        REQUIRE_FALSE(node1 == nullptr);
        REQUIRE(node2 == nullptr);
        REQUIRE_FALSE(edge1 == nullptr);
        REQUIRE_FALSE(edge2 == nullptr);
    }

    SECTION("Weighted Directed Graph") {
        // arrange
        WeightedDirectedGraph g;
        g.addNode(1);
        g.addNode(2);
        g.addEdge(1, 2, 5);
        const auto& cg = g;

        // act
        const Node* node1 = cg.getNode(1);
        const Node* node2 = cg.getNode(999);
        const Edge* edge1 = cg.getEdge(1, 2);
        const Edge* edge2 = cg.getEdge(2, 1);

        // assert
        REQUIRE_FALSE(node1 == nullptr);
        REQUIRE(node2 == nullptr);
        REQUIRE_FALSE(edge1 == nullptr);
        REQUIRE(edge1->getWeight() == 5);
        REQUIRE(edge2 == nullptr);
    }

    SECTION("Weighted Undirected Graph") {
        // arrange
        WeightedUndirectedGraph g;
        g.addNode(1);
        g.addNode(2);
        g.addEdge(1, 2, 10);
        const auto& cg = g;

        // act
        const Node* node1 = cg.getNode(1);
        const Node* node2 = cg.getNode(999);
        const Edge* edge1 = cg.getEdge(1, 2);
        const Edge* edge2 = cg.getEdge(2, 1);

        // assert
        REQUIRE_FALSE(node1 == nullptr);
        REQUIRE(node2 == nullptr);
        REQUIRE_FALSE(edge1 == nullptr);
        REQUIRE(edge1->getWeight() == 10);
        REQUIRE_FALSE(edge2 == nullptr);
    }
}

TEST_CASE("Testing addNode(position)", "[Graph]") {

    SECTION("Unweighted Directed Graph") {
        // arrange
        UnweightedDirectedGraph g;

        // act
        Node* node1 = g.addNode(1.5, 2.5);
        Node* node2 = g.addNode(3.0, 4.0);

        // assert
        REQUIRE_FALSE(node1 == nullptr);
        REQUIRE_FALSE(node2 == nullptr);
    }

    SECTION("Unweighted Undirected Graph") {
        // arrange
        UnweightedUndirectedGraph g;

        // act
        Node* node1 = g.addNode(1.5, 2.5);
        Node* node2 = g.addNode(3.0, 4.0);

        // assert
        REQUIRE_FALSE(node1 == nullptr);
        REQUIRE_FALSE(node2 == nullptr);
    }

    SECTION("Weighted Directed Graph") {
        // arrange
        WeightedDirectedGraph g;

        // act
        Node* node1 = g.addNode(1.5, 2.5);
        Node* node2 = g.addNode(3.0, 4.0);

        // assert
        REQUIRE_FALSE(node1 == nullptr);
        REQUIRE_FALSE(node2 == nullptr);
    }

    SECTION("Weighted Undirected Graph") {
        // arrange
        WeightedUndirectedGraph g;

        // act
        Node* node1 = g.addNode(1.5, 2.5);
        Node* node2 = g.addNode(3.0, 4.0);

        // assert
        REQUIRE_FALSE(node1 == nullptr);
        REQUIRE_FALSE(node2 == nullptr);
    }
}
