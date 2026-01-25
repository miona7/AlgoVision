#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "AStar.h"
#include "WeightedDirectedGraph.h"

TEST_CASE("A*: simple path exists", "[AStar]") {
    // arrange
    auto g = std::make_shared<WeightedDirectedGraph>();

    for(unsigned i = 1; i <= 4; ++i) {
        g->addNode(i);
    }

    g->addEdge(1, 2, 1);
    g->addEdge(2, 3, 2);
    g->addEdge(3, 4, 3);
    g->addEdge(1, 4, 10);

    AStar                 astar(g);
    std::vector<unsigned> expectedPath = {1, 2, 3, 4};
    int                   expectedCost = 6;

    // act
    REQUIRE_NOTHROW(astar.execute(1, 4));

    // assert
    std::vector<unsigned> path = astar.getPath();
    int                   cost = astar.getTotalCost();

    REQUIRE(path == expectedPath);
    REQUIRE(cost == expectedCost);
}

TEST_CASE("A*: finds shortest path in simple graph", "[AStar]") {
    // arrange
    auto graph = std::make_shared<WeightedDirectedGraph>();

    graph->addNode(1, 0, 0);
    graph->addNode(2, 1, 0);
    graph->addNode(3, 1, 1);
    graph->addNode(4, 2, 1);

    graph->addEdge(1, 2, 1);
    graph->addEdge(2, 4, 2);
    graph->addEdge(1, 3, 2);
    graph->addEdge(3, 4, 1);

    AStar                 astar(graph);
    std::vector<unsigned> expectedPath1 = {1, 2, 4};
    std::vector<unsigned> expectedPath2 = {1, 3, 4};
    int                   expectedCost  = 3;

    // act
    REQUIRE_NOTHROW(astar.execute(1, 4));

    // assert
    std::vector<unsigned> path      = astar.getPath();
    int                   cost      = astar.getTotalCost();
    bool                  validPath = path == expectedPath1 || path == expectedPath2;

    REQUIRE(validPath);
    REQUIRE(cost == expectedCost);
}

TEST_CASE("A*: no path exists", "[AStar]") {
    // arrange
    auto g = std::make_shared<WeightedDirectedGraph>();

    g->addNode(1);
    g->addNode(2);

    AStar astar(g);

    // act + assert
    REQUIRE_THROWS_WITH(astar.execute(1, 2), "No path found from start to goal!");
}

TEST_CASE("A*: negative edge weight", "[AStar]") {
    // arrange
    auto g = std::make_shared<WeightedDirectedGraph>();

    g->addNode(1);
    g->addNode(2);
    g->addEdge(1, 2, -5);

    AStar astar(g);

    // act + assert
    REQUIRE_THROWS_WITH(astar.execute(1, 2), "Graph contains negative edge weights!");
}

static const char* stepTypeToString(StepType t) {
    switch(t) {
    case StepType::VisitNode:
        return "VisitNode";
    case StepType::ProcessNode:
        return "ProcessNode";
    case StepType::ExamineEdge:
        return "ExamineEdge";
    case StepType::SelectEdge:
        return "SelectEdge";
    case StepType::UpdateDistance:
        return "UpdateDistance";
    case StepType::AddToPath:
        return "AddToPath";
    }
    return nullptr;
}

void runAStarLoggingTest(const std::shared_ptr<WeightedDirectedGraph> g, unsigned startNode,
                         unsigned goalNode) {
    // arrange
    AStar                       astar(g);
    const std::vector<StepType> expectedSteps = {StepType::VisitNode,  StepType::ProcessNode,
                                                 StepType::AddToPath,  StepType::ExamineEdge,
                                                 StepType::SelectEdge, StepType::UpdateDistance};

    // act
    REQUIRE_NOTHROW(astar.execute(startNode, goalNode));

    const auto& steps = astar.getSteps();
    std::cout << std::endl << "Total steps produced: " << steps.size() << std::endl;

    // assert
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

TEST_CASE("A*: steps test", "[AStar]") {
    // arrange
    auto g = std::make_shared<WeightedDirectedGraph>();

    g->addNode(1, 0, 0);
    g->addNode(2, 1, 0);
    g->addNode(3, 1, 1);
    g->addNode(4, 2, 1);

    g->addEdge(1, 2, 1);
    g->addEdge(2, 4, 2);
    g->addEdge(1, 3, 2);
    g->addEdge(3, 4, 1);

    // act + assert
    runAStarLoggingTest(g, 1, 4);
}
