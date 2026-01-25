#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "FloydWarshall.h"
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
    default:
        return nullptr;
    }
}

void runFloydWarshallLoggingTest(const std::shared_ptr<Graph>& g) {
    FloydWarshall               fw(g);
    const std::vector<StepType> expectedSteps = {StepType::VisitNode, StepType::ProcessNode,
                                                 StepType::ExamineEdge, StepType::UpdateDistance};

    REQUIRE_NOTHROW(fw.execute());

    const auto& steps = fw.getSteps();
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

TEST_CASE("Floyd-Warshall works on directed weighted graph", "[FLOYD_WARSHALL]") {
    auto g = std::make_shared<WeightedDirectedGraph>();

    for(unsigned i = 1; i <= 4; ++i) {
        g->addNode(i);
    }

    g->addEdge(1, 2, 3);
    g->addEdge(2, 3, 4);
    g->addEdge(1, 3, 10);
    g->addEdge(3, 4, 2);

    REQUIRE_NOTHROW(runFloydWarshallLoggingTest(g));
}

TEST_CASE("Floyd-Warshall throws on undirected graph", "[FLOYD_WARSHALL]") {
    auto g = std::make_shared<WeightedUndirectedGraph>();

    g->addNode(1);
    g->addNode(2);
    g->addEdge(1, 2, 5);

    FloydWarshall fw(g);
    REQUIRE_THROWS_AS(fw.execute(), std::runtime_error);
}

TEST_CASE("Floyd-Warshall throws on empty graph", "[FLOYD_WARSHALL]") {
    auto g = std::make_shared<WeightedDirectedGraph>();

    FloydWarshall fw(g);
    REQUIRE_THROWS_AS(fw.execute(), std::runtime_error);
}

TEST_CASE("Floyd-Warshall detects negative cycle", "[FLOYD_WARSHALL]") {
    auto g = std::make_shared<WeightedDirectedGraph>();

    g->addNode(1);
    g->addNode(2);
    g->addNode(3);

    g->addEdge(1, 2, 1);
    g->addEdge(2, 3, -2);
    g->addEdge(3, 1, -2);

    FloydWarshall fw(g);
    REQUIRE_THROWS_AS(fw.execute(), std::runtime_error);
}
