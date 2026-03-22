#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include <iostream>

#include "BellmanFord.h"
#include "UnweightedDirectedGraph.h"
#include "WeightedDirectedGraph.h"

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
    case StepType::RelaxEdge:
        return "RelaxEdge";
    case StepType::UpdateDistance:
        return "UpdateDistance";
    default:
        return nullptr;
    }
}

void runBellmanFordLoggingTest(const std::shared_ptr<Graph>& g, unsigned startNode) {
    BellmanFord                 bf(g);
    const std::vector<StepType> expectedSteps = {StepType::VisitNode, StepType::ProcessNode,
                                                 StepType::ExamineEdge, StepType::RelaxEdge,
                                                 StepType::UpdateDistance};

    auto err = bf.execute(startNode);
    REQUIRE_SUCCESS(err);

    const auto& steps = bf.getSteps();
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

TEST_CASE("Bellman-Ford throws on invalid start node", "[BELLMAN_FORD]") {
    auto g = std::make_shared<WeightedDirectedGraph>();

    g->addNode(1);
    g->addNode(2);
    g->addEdge(1, 2, 5);

    BellmanFord bf(g);
    auto        err = bf.execute(0);

    REQUIRE_ERROR(err, AlgorithmErrorType::StartNodeMissing,
                  "Start node does not exist in the graph.");
}

TEST_CASE("Bellman-Ford throws on unweighted graph", "[BELLMAN_FORD]") {
    auto g = std::make_shared<UnweightedDirectedGraph>();

    g->addNode(1);
    g->addNode(2);
    g->addEdge(1, 2);

    BellmanFord bf(g);
    auto        err = bf.execute(1);

    REQUIRE_ERROR(err, AlgorithmErrorType::GraphTypeInvalid, "Graph type is invalid.");
}

TEST_CASE("Bellman-Ford detects negative cycle", "[BELLMAN_FORD]") {
    auto g = std::make_shared<WeightedDirectedGraph>();

    g->addNode(1);
    g->addNode(2);
    g->addNode(3);

    g->addEdge(1, 2, 1);
    g->addEdge(2, 3, -1);
    g->addEdge(3, 1, -1);

    BellmanFord bf(g);
    auto        err = bf.execute(1);

    REQUIRE_SUCCESS(err);
    REQUIRE(bf.hasNegativeCycle());
}

TEST_CASE("Bellman-Ford on weighted directed graph", "[BELLMAN_FORD]") {
    auto g = std::make_shared<WeightedDirectedGraph>();

    for(unsigned i = 1; i <= 5; ++i) {
        g->addNode(i);
    }

    g->addEdge(1, 2, 6);
    g->addEdge(1, 3, 7);
    g->addEdge(2, 3, 8);
    g->addEdge(2, 4, 5);
    g->addEdge(2, 5, -4);
    g->addEdge(3, 4, -3);
    g->addEdge(3, 5, 9);
    g->addEdge(4, 2, -2);
    g->addEdge(5, 4, 7);

    runBellmanFordLoggingTest(g, 1);
}
