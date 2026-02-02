#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "Dijkstra.h"
#include "UnweightedDirectedGraph.h"
#include "UnweightedUndirectedGraph.h"
#include "WeightedDirectedGraph.h"
#include "WeightedUndirectedGraph.h"

static void REQUIRE_SUCCESS(const std::optional<AlgorithmError>& err) {
    REQUIRE_FALSE(err.has_value());
}

static void REQUIRE_ERROR(const std::optional<AlgorithmError>& err, AlgorithmErrorType expectedType,
                          const std::string& expectedMessage) {
    REQUIRE(err.has_value());
    REQUIRE(err->m_type == expectedType);
    REQUIRE(err->m_message == expectedMessage);
}

static const char* stepTypeToString(StepType t) {
    switch(t) {
    case StepType::VisitNode:
        return "VisitNode";
    case StepType::ProcessNode:
        return "ProcessNode";
    case StepType::ExamineEdge:
        return "ExamineEdge";
    case StepType::UpdateDistance:
        return "UpdateDistance";
    }
    return nullptr;
}

void runDijkstraLoggingTest(const std::shared_ptr<Graph> g, unsigned startNode) {
    // arrange
    Dijkstra                    dijkstra(g);
    const std::vector<StepType> expectedSteps = {StepType::VisitNode, StepType::ProcessNode,
                                                 StepType::ExamineEdge, StepType::UpdateDistance};

    // act
    auto err = dijkstra.execute(startNode);

    // assert
    REQUIRE_SUCCESS(err);

    const auto& steps = dijkstra.getSteps();
    std::cout << std::endl << "Total steps produced: " << steps.size() << std::endl;

    REQUIRE_FALSE(steps.empty());

    for(size_t i = 0; i < steps.size(); ++i) {
        const auto& s = steps[i];
        std::cout << "[" << i << "] " << stepTypeToString(s.m_type);

        if(s.m_node) {
            std::cout << " | node = " << *s.m_node;
        }
        if(s.m_from && s.m_to) {
            std::cout << " | edge = " << *s.m_from << " -> " << *s.m_to;
        }
        std::cout << std::endl;
    }

    for(auto expected: expectedSteps) {
        bool found = std::any_of(steps.begin(), steps.end(),
                                 [&](const auto& s) { return s.m_type == expected; });
        REQUIRE(found);
    }
}

TEST_CASE("Dijkstra on Unweighted Directed Graph", "[DIJKSTRA]") {
    auto udg = std::make_shared<UnweightedDirectedGraph>();
    for(unsigned i = 1; i <= 5; ++i) {
        udg->addNode(i);
    }

    udg->addEdge(1, 2);
    udg->addEdge(1, 3);
    udg->addEdge(2, 4);
    udg->addEdge(3, 4);
    udg->addEdge(4, 5);

    Dijkstra dijkstra(udg);

    auto err = dijkstra.execute(1);

    REQUIRE_SUCCESS(err);
}

TEST_CASE("Dijkstra on Unweighted Undirected Graph", "[DIJKSTRA]") {
    auto uug = std::make_shared<UnweightedUndirectedGraph>();
    for(unsigned i = 1; i <= 5; ++i) {
        uug->addNode(i);
    }

    uug->addEdge(1, 2);
    uug->addEdge(1, 3);
    uug->addEdge(2, 4);
    uug->addEdge(3, 4);
    uug->addEdge(4, 5);

    Dijkstra dijkstra(uug);

    auto err = dijkstra.execute(1);

    REQUIRE_SUCCESS(err);
}

TEST_CASE("Dijkstra on Weighted Directed Graph", "[DIJKSTRA]") {
    auto wdg = std::make_shared<WeightedDirectedGraph>();
    for(unsigned i = 1; i <= 6; ++i) {
        wdg->addNode(i);
    }

    wdg->addEdge(1, 2, 2);
    wdg->addEdge(1, 3, 5);
    wdg->addEdge(2, 4, 1);
    wdg->addEdge(3, 4, 2);
    wdg->addEdge(4, 5, 1);
    wdg->addEdge(5, 6, 3);

    Dijkstra dijkstra(wdg);

    auto err = dijkstra.execute(1);

    REQUIRE_SUCCESS(err);
}

TEST_CASE("Dijkstra on Weighted Undirected Graph", "[DIJKSTRA]") {
    auto wug = std::make_shared<WeightedUndirectedGraph>();
    for(unsigned i = 1; i <= 5; ++i) {
        wug->addNode(i);
    }

    wug->addEdge(1, 2, 2);
    wug->addEdge(1, 3, 4);
    wug->addEdge(2, 4, 1);
    wug->addEdge(3, 4, 3);
    wug->addEdge(4, 5, 5);

    Dijkstra dijkstra(wug);

    auto err = dijkstra.execute(1);

    REQUIRE_SUCCESS(err);
}

TEST_CASE("Dijkstra with invalid start node", "[DIJKSTRA]") {
    auto udg = std::make_shared<UnweightedDirectedGraph>();
    udg->addNode(1);
    udg->addNode(2);

    Dijkstra dijkstra(udg);

    auto err = dijkstra.execute(0);

    REQUIRE_ERROR(err, AlgorithmErrorType::StartNodeMissing,
                  "Start node does not exist in the graph.");
}

TEST_CASE("Dijkstra on graph with negative edges", "[DIJKSTRA]") {
    auto wdg = std::make_shared<WeightedDirectedGraph>();
    for(unsigned i = 1; i <= 3; ++i) {
        wdg->addNode(i);
    }

    wdg->addEdge(1, 2, -5);
    wdg->addEdge(2, 3, 2);

    Dijkstra dijkstra(wdg);

    auto err = dijkstra.execute(1);

    REQUIRE_ERROR(err, AlgorithmErrorType::NegativeEdgeWeights,
                  "Graph contains edge with negative weight.");
}

TEST_CASE("Dijkstra steps test on all graph types", "[DIJKSTRA]") {

    SECTION("Unweighted Directed Graph") {
        auto udg = std::make_shared<UnweightedDirectedGraph>();

        for(unsigned i = 1; i <= 5; ++i) {
            udg->addNode(i);
        }

        udg->addEdge(1, 2);
        udg->addEdge(1, 3);
        udg->addEdge(2, 4);
        udg->addEdge(3, 4);
        udg->addEdge(4, 5);

        runDijkstraLoggingTest(udg, 1);
    }

    SECTION("Unweighted Unirected Graph") {
        auto uug = std::make_shared<UnweightedUndirectedGraph>();

        for(unsigned i = 1; i <= 5; ++i) {
            uug->addNode(i);
        }

        uug->addEdge(1, 2);
        uug->addEdge(1, 3);
        uug->addEdge(2, 4);
        uug->addEdge(3, 4);
        uug->addEdge(4, 5);

        runDijkstraLoggingTest(uug, 1);
    }

    SECTION("Weighted Directed Graph") {
        auto wdg = std::make_shared<WeightedDirectedGraph>();

        for(unsigned i = 1; i <= 6; ++i) {
            wdg->addNode(i);
        }

        wdg->addEdge(1, 2, 2);
        wdg->addEdge(1, 3, 5);
        wdg->addEdge(2, 4, 1);
        wdg->addEdge(3, 4, 2);
        wdg->addEdge(4, 5, 1);
        wdg->addEdge(5, 6, 3);

        runDijkstraLoggingTest(wdg, 1);
    }

    SECTION("Weighted Undirected Graph") {
        auto wug = std::make_shared<WeightedUndirectedGraph>();

        for(unsigned i = 1; i <= 5; ++i) {
            wug->addNode(i);
        }

        wug->addEdge(1, 2, 2);
        wug->addEdge(1, 3, 4);
        wug->addEdge(2, 4, 1);
        wug->addEdge(3, 4, 3);
        wug->addEdge(4, 5, 5);

        runDijkstraLoggingTest(wug, 1);
    }
}
