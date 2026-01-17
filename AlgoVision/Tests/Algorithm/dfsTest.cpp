#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "DFS.h"
#include "UnweightedDirectedGraph.h"
#include "UnweightedUndirectedGraph.h"
#include "WeightedDirectedGraph.h"
#include "WeightedUndirectedGraph.h"

TEST_CASE("DFS on Unweighted Directed Graph", "[DFS]") {
    auto graph = std::make_shared<UnweightedDirectedGraph>();

    for(unsigned i = 1; i <= 5; ++i) {
        graph->addNode(i);
    }

    graph->addEdge(1, 2);
    graph->addEdge(1, 3);
    graph->addEdge(2, 4);
    graph->addEdge(3, 5);

    DFS dfs(graph);
    REQUIRE_NOTHROW(dfs.execute(1));
}

TEST_CASE("DFS on Unweighted Undirected Graph", "[DFS]") {
    auto graph = std::make_shared<UnweightedUndirectedGraph>();

    for(unsigned i = 1; i <= 5; ++i) {
        graph->addNode(i);
    }

    graph->addEdge(1, 2);
    graph->addEdge(1, 3);
    graph->addEdge(2, 4);
    graph->addEdge(3, 5);

    DFS dfs(graph);
    REQUIRE_NOTHROW(dfs.execute(1));
}

TEST_CASE("DFS on Weighted Directed Graph", "[DFS]") {
    auto graph = std::make_shared<WeightedDirectedGraph>();

    for(unsigned i = 1; i <= 4; ++i) {
        graph->addNode(i);
    }

    graph->addEdge(1, 2, 10);
    graph->addEdge(1, 3, 5);
    graph->addEdge(2, 4, 1);
    graph->addEdge(3, 4, 2);

    DFS dfs(graph);
    REQUIRE_NOTHROW(dfs.execute(1));
}

TEST_CASE("DFS on Weighted Undirected Graph", "[DFS]") {
    auto graph = std::make_shared<WeightedUndirectedGraph>();

    for(unsigned i = 1; i <= 4; ++i) {
        graph->addNode(i);
    }

    graph->addEdge(1, 2, 3);
    graph->addEdge(1, 3, 7);
    graph->addEdge(2, 4, 1);
    graph->addEdge(3, 4, 2);

    DFS dfs(graph);
    REQUIRE_NOTHROW(dfs.execute(1));
}

TEST_CASE("DFS throws for invalid start node", "[DFS]") {
    auto graph = std::make_shared<UnweightedDirectedGraph>();
    graph->addNode(1);
    graph->addNode(2);

    DFS dfs(graph);
    REQUIRE_THROWS_AS(dfs.execute(0), std::runtime_error);
}

static const char* stepTypeToString(StepType t) {
    switch(t) {
    case StepType::Start:
        return "Start";
    case StepType::Finish:
        return "Finish";
    case StepType::VisitNode:
        return "VisitNode";
    case StepType::ProcessNode:
        return "ProcessNode";
    case StepType::MarkNode:
        return "MarkNode";
    case StepType::ExamineEdge:
        return "ExamineEdge";
    case StepType::PushToStack:
        return "PushToStack";
    case StepType::PopFromStack:
        return "PopFromStack";
    }
    return nullptr;
}

void runDFSLoggingTest(const std::shared_ptr<Graph>& g, unsigned startNode) {
    // arrange
    DFS                         dfs(g);
    const std::vector<StepType> expectedSteps = {
        StepType::Start,       StepType::Finish,      StepType::VisitNode,   StepType::ProcessNode,
        StepType::ExamineEdge, StepType::PushToStack, StepType::PopFromStack};

    // act
    dfs.execute(startNode);

    const auto& steps   = dfs.getSteps();
    const auto& visited = dfs.getVisited();
    std::cout << std::endl << "Total steps produced: " << steps.size() << std::endl;

    // assert
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
        bool found = std::any_of(steps.begin(), steps.end(),
                                 [&](const auto& s) { return s.m_type == expected; });
        REQUIRE(found);
    }
}

TEST_CASE("UDG", "[DFS]") {
    auto g = std::make_shared<UnweightedDirectedGraph>();

    for(unsigned i = 0; i <= 2; ++i) {
        g->addNode(i);
    }

    g->addEdge(0, 1);
    g->addEdge(1, 2);

    runDFSLoggingTest(g, 0);
}

TEST_CASE("UUG", "[DFS]") {
    auto g = std::make_shared<UnweightedUndirectedGraph>();

    for(unsigned i = 0; i <= 2; ++i) {
        g->addNode(i);
    }

    g->addEdge(0, 1);
    g->addEdge(1, 2);

    runDFSLoggingTest(g, 0);
}

TEST_CASE("WDG", "[DFS]") {
    auto g = std::make_shared<WeightedDirectedGraph>();

    for(unsigned i = 0; i <= 2; ++i) {
        g->addNode(i);
    }

    g->addEdge(0, 1, 5);
    g->addEdge(1, 2, 7);

    runDFSLoggingTest(g, 0);
}

TEST_CASE("WUG", "[DFS]") {
    auto g = std::make_shared<WeightedUndirectedGraph>();

    for(unsigned i = 0; i <= 2; ++i) {
        g->addNode(i);
    }

    g->addEdge(0, 1, 3);
    g->addEdge(1, 2, 4);

    runDFSLoggingTest(g, 0);
}
