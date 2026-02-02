#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "BFS.h"
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
    default:
        return nullptr;
    }
}

void runBFSLoggingTest(const std::shared_ptr<Graph>& g, unsigned startNode) {
    BFS                         bfs(g);
    const std::vector<StepType> expectedSteps = {StepType::VisitNode, StepType::ProcessNode,
                                                 StepType::ExamineEdge};

    auto err = bfs.execute(startNode);

    REQUIRE_SUCCESS(err);

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
        std::cout << std::endl;
    }

    for(auto expected: expectedSteps) {
        bool found = std::any_of(steps.begin(), steps.end(),
                                 [&](const auto& s) { return s.m_type == expected; });
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

    BFS  bfs(g);
    auto err = bfs.execute(0);

    REQUIRE_SUCCESS(err);
    runBFSLoggingTest(g, 0);
}

TEST_CASE("BFS on Unweighted Undirected Graph", "[BFS]") {
    auto g = std::make_shared<UnweightedUndirectedGraph>();

    for(unsigned i = 0; i <= 2; ++i) {
        g->addNode(i);
    }

    g->addEdge(0, 1);
    g->addEdge(1, 2);

    BFS  bfs(g);
    auto err = bfs.execute(0);

    REQUIRE_SUCCESS(err);
    runBFSLoggingTest(g, 0);
}

TEST_CASE("BFS on Weighted Directed Graph", "[BFS]") {
    auto g = std::make_shared<WeightedDirectedGraph>();

    for(unsigned i = 0; i <= 2; ++i) {
        g->addNode(i);
    }

    g->addEdge(0, 1, 5);
    g->addEdge(1, 2, 7);

    BFS  bfs(g);
    auto err = bfs.execute(0);

    REQUIRE_SUCCESS(err);
    runBFSLoggingTest(g, 0);
}

TEST_CASE("BFS on Weighted Undirected Graph", "[BFS]") {
    auto g = std::make_shared<WeightedUndirectedGraph>();

    for(unsigned i = 0; i <= 2; ++i) {
        g->addNode(i);
    }

    g->addEdge(0, 1, 3);
    g->addEdge(1, 2, 4);

    BFS  bfs(g);
    auto err = bfs.execute(0);

    REQUIRE_SUCCESS(err);
    runBFSLoggingTest(g, 0);
}

TEST_CASE("BFS throws for invalid start node", "[BFS]") {
    auto g = std::make_shared<UnweightedDirectedGraph>();
    g->addNode(1);
    g->addNode(2);

    BFS  bfs(g);
    auto err = bfs.execute(0);
    REQUIRE_ERROR(err, AlgorithmErrorType::StartNodeMissing,
                  "Start node does not exist in the graph.");
}
