#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "DFS.h"
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
    }
    return nullptr;
}

void runDFSLoggingTest(const std::shared_ptr<Graph> g, unsigned startNode) {
    // arrange
    DFS                         dfs(g);
    const std::vector<StepType> expectedSteps = {StepType::VisitNode, StepType::ProcessNode,
                                                 StepType::ExamineEdge};

    // act
    auto err = dfs.execute(startNode);

    // assert
    REQUIRE_SUCCESS(err);

    const auto& steps   = dfs.getSteps();
    const auto& visited = dfs.getVisited();
    std::cout << std::endl << "Total steps produced: " << steps.size() << std::endl;

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
        std::cout << std::endl;
    }

    for(auto expected: expectedSteps) {
        bool found = std::any_of(steps.begin(), steps.end(),
                                 [&](const auto& s) { return s.m_type == expected; });
        REQUIRE(found);
    }

    REQUIRE(visited.at(startNode));
}

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

    auto err = dfs.execute(1);

    REQUIRE_SUCCESS(err);
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

    auto err = dfs.execute(1);

    REQUIRE_SUCCESS(err);
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

    auto err = dfs.execute(1);

    REQUIRE_SUCCESS(err);
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

    auto err = dfs.execute(1);

    REQUIRE_SUCCESS(err);
}

TEST_CASE("DFS throws for invalid start node", "[DFS]") {
    auto graph = std::make_shared<UnweightedDirectedGraph>();
    graph->addNode(1);
    graph->addNode(2);

    DFS dfs(graph);

    auto err = dfs.execute(0);

    REQUIRE_ERROR(err, AlgorithmErrorType::StartNodeMissing,
                  "Start node does not exist in the graph.");
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
