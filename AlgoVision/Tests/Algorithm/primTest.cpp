#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "Prim.h"
#include "WeightedDirectedGraph.h"
#include "WeightedUndirectedGraph.h"

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
    case StepType::SelectEdge:
        return "SelectEdge";
    default:
        return nullptr;
    }
}

void runPrimLoggingTest(const std::shared_ptr<Graph>& g) {
    Prim                        prim(g);
    const std::vector<StepType> expectedSteps = {StepType::VisitNode, StepType::ProcessNode,
                                                 StepType::ExamineEdge, StepType::UpdateDistance,
                                                 StepType::SelectEdge};

    REQUIRE_NOTHROW(prim.execute());

    const auto& steps = prim.getSteps();
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
        if(s.m_value) {
            std::cout << " | value = " << *s.m_value;
        }
        if(s.m_message) {
            std::cout << " | msg = \"" << *s.m_message << "\"";
        }

        std::cout << std::endl;
    }

    for(auto expected: expectedSteps) {
        bool found = std::any_of(steps.begin(), steps.end(),
                                 [&](const auto& s) { return s.m_type == expected; });
        REQUIRE(found);
    }
}

TEST_CASE("Prim works on connected undirected weighted graph", "[PRIM]") {
    auto g = std::make_shared<WeightedUndirectedGraph>();

    g->addNode(1);
    g->addNode(2);
    g->addNode(3);
    g->addNode(4);

    g->addEdge(1, 2, 1);
    g->addEdge(2, 3, 2);
    g->addEdge(3, 4, 3);
    g->addEdge(1, 4, 10);

    REQUIRE_NOTHROW(runPrimLoggingTest(g));
}

TEST_CASE("Prim throws on disconnected graph", "[PRIM]") {
    auto g = std::make_shared<WeightedUndirectedGraph>();

    g->addNode(1);
    g->addNode(2);
    g->addNode(3);

    g->addEdge(1, 2, 1); // node 3 isolated

    Prim prim(g);
    REQUIRE_THROWS_AS(prim.execute(), std::runtime_error);
}

TEST_CASE("Prim throws on directed graph", "[PRIM]") {
    auto g = std::make_shared<WeightedDirectedGraph>();

    g->addNode(1);
    g->addNode(2);

    g->addEdge(1, 2, 5);

    Prim prim(g);
    REQUIRE_THROWS_AS(prim.execute(), std::runtime_error);
}

TEST_CASE("Prim throws on empty graph", "[PRIM]") {
    auto g = std::make_shared<WeightedUndirectedGraph>();

    Prim prim(g);
    REQUIRE_THROWS_AS(prim.execute(), std::runtime_error);
}
