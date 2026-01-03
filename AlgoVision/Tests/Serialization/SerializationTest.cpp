#include <catch2/catch_test_macros.hpp>

#include "Serializer.h"
#include "WeightedDirectedGraph.h"

#include <QDir>
#include <QFile>
#include <iostream>

TEST_CASE("SerializationTest - Graph save/load", "[serialization]") {
    std::cout << "\n==============================\n";
    std::cout << "STARTING SERIALIZATION TEST\n";
    std::cout << "==============================\n";

    WeightedDirectedGraph graph;
    graph.addNode(1, 10.0, 20.0);
    graph.addNode(2, 30.0, 40.0);
    graph.addNode(3, 50.0, 60.0);
    graph.addNode(4, 12.0, 45.0);

    graph.addEdge(1, 2, 5);
    graph.addEdge(2, 3, 7);

    std::cout << "Graph created.\n";
    std::cout << "Nodes: " << graph.getNodes().size()
              << ", Edges: " << graph.getEdges().size() << "\n";

    const QString filePath =
        QDir::currentPath() + "/serializationTestGraph.json";

    std::cout << "Saving JSON to:\n";
    std::cout << ">> " << filePath.toStdString() << "\n";

    Serializer serializer;
    serializer.save(graph, filePath, graph.isWeighted(), graph.isDirected());

    REQUIRE(QFile::exists(filePath));
    std::cout << "Save SUCCESS.\n";

    QFile file(filePath);
    if (file.open(QFile::ReadOnly)) {
        std::cout << "\n----- JSON FILE CONTENT -----\n";
        std::cout << file.readAll().toStdString() << "\n";
        std::cout << "-----------------------------\n";
        file.close();
    } else {
        std::cout << "ERROR: Could not open JSON file.\n";
    }

    WeightedDirectedGraph loadedGraph;
    bool isWeighted = false;
    bool isDirected = false;

    serializer.load(loadedGraph, filePath, isWeighted, isDirected);

    std::cout << "Load finished.\n";
    std::cout << "Loaded graph: nodes="
              << loadedGraph.getNodes().size()
              << ", edges="
              << loadedGraph.getEdges().size()
              << "\n";

    REQUIRE(isWeighted == true);
    REQUIRE(isDirected == true);

    REQUIRE(loadedGraph.getNodes().size() == graph.getNodes().size());
    REQUIRE(loadedGraph.getEdges().size() == graph.getEdges().size());

    for (const auto& [id, edge] : graph.getEdges()) {
        const auto& e = loadedGraph.getEdges().at(id);

        REQUIRE(edge.startNode() == e.startNode());
        REQUIRE(edge.endNode()   == e.endNode());
        REQUIRE(edge.getWeight() == e.getWeight());
    }

    std::cout << "SERIALIZATION TEST PASSED.\n";
    std::cout << "==============================\n";
}
