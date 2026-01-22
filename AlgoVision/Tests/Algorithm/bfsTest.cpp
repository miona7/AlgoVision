#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "BFS.h"
#include "UnweightedDirectedGraph.h"
#include "UnweightedUndirectedGraph.h"
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
    default:
        return nullptr;
    }
}

void runBFSLoggingTest(const std::shared_ptr<Graph>& g, unsigned startNode) {
    BFS bfs(g);
    const std::vector<StepType> expectedSteps = {
        StepType::VisitNode,
        StepType::ProcessNode,
        StepType::ExamineEdge
    };

    bfs.execute(startNode);

    const auto& steps   = bfs.getSteps();
    const auto& visited = bfs.getVisited();

    REQUIRE_FALSE(steps.empty());

    for(std::size_t i = 0; i < steps.size(); ++i) {
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
        bool found = std::any_of(
            steps.begin(), steps.end(),
            [&](const auto& s) { return s.m_type == expected; }
            );
        REQUIRE(found);
    }

    REQUIRE(visited.at(startNode));
}


TEST_CASE("BFS on Unweighted Directed Graph", "[BFS]") {
    auto g = std::make_shared<UnweightedDirectedGraph>();

    for(unsigned i = 0; i <= 2; ++i) {
        g->addNode(i);
    }

    g->addEdge(0, 1);
    g->addEdge(1, 2);

    REQUIRE_NOTHROW(runBFSLoggingTest(g, 0));
}

TEST_CASE("BFS on Unweighted Undirected Graph", "[BFS]") {
    auto g = std::make_shared<UnweightedUndirectedGraph>();

    for(unsigned i = 0; i <= 2; ++i) {
        g->addNode(i);
    }

    g->addEdge(0, 1);
    g->addEdge(1, 2);

    REQUIRE_NOTHROW(runBFSLoggingTest(g, 0));
}

TEST_CASE("BFS on Weighted Directed Graph", "[BFS]") {
    auto g = std::make_shared<WeightedDirectedGraph>();

    for(unsigned i = 0; i <= 2; ++i) {
        g->addNode(i);
    }

    g->addEdge(0, 1, 5);
    g->addEdge(1, 2, 7);

    REQUIRE_NOTHROW(runBFSLoggingTest(g, 0));
}

TEST_CASE("BFS on Weighted Undirected Graph", "[BFS]") {
    auto g = std::make_shared<WeightedUndirectedGraph>();

    for(unsigned i = 0; i <= 2; ++i) {
        g->addNode(i);
    }

    g->addEdge(0, 1, 3);
    g->addEdge(1, 2, 4);

    REQUIRE_NOTHROW(runBFSLoggingTest(g, 0));
}

TEST_CASE("BFS throws for invalid start node", "[BFS]") {
    auto g = std::make_shared<UnweightedDirectedGraph>();
    g->addNode(1);
    g->addNode(2);

    BFS bfs(g);
    REQUIRE_THROWS_AS(bfs.execute(0), std::runtime_error);
}
