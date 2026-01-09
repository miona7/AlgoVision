#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "AStar.h"
#include "WeightedDirectedGraph.h"

TEST_CASE("AStar: simple path exists", "[AStar]") {
    auto g = std::make_shared<WeightedDirectedGraph>();
    for(unsigned i = 1; i <= 4; ++i) {
        g->addNode(i);
    }

    g->addEdge(1, 2, 1);
    g->addEdge(2, 3, 2);
    g->addEdge(3, 4, 3);
    g->addEdge(1, 4, 10);

    AStar astar(g);
    REQUIRE_NOTHROW(astar.execute(1, 4));

    std::vector<unsigned> expectedPath = {1, 2, 3, 4};
    REQUIRE(astar.getPath() == expectedPath);
    REQUIRE(astar.getTotalCost() == 6);
}

TEST_CASE("A* finds shortest path in simple graph", "[AStar]") {
    auto graph = std::make_shared<WeightedDirectedGraph>();

    graph->addNode(1, 0, 0);
    graph->addNode(2, 1, 0);
    graph->addNode(3, 1, 1);
    graph->addNode(4, 2, 1);

    graph->addEdge(1, 2, 1);
    graph->addEdge(2, 4, 2);
    graph->addEdge(1, 3, 2);
    graph->addEdge(3, 4, 1);

    AStar astar(graph);
    REQUIRE_NOTHROW(astar.execute(1, 4));

    std::vector<unsigned> expectedPath = {1, 2, 4}; // {1, 3, 4}
    REQUIRE(astar.getPath() == expectedPath);
    REQUIRE(astar.getTotalCost() == 3);
}

TEST_CASE("AStar: no path exists", "[AStar]") {
    auto g = std::make_shared<WeightedDirectedGraph>();
    g->addNode(1);
    g->addNode(2);

    AStar astar(g);
    REQUIRE_THROWS_WITH(astar.execute(1, 2), "No path found from start to goal!");
}

TEST_CASE("AStar: negative edge weight", "[AStar]") {
    auto g = std::make_shared<WeightedDirectedGraph>();
    g->addNode(1);
    g->addNode(2);
    g->addEdge(1, 2, -5);

    AStar astar(g);
    REQUIRE_THROWS_WITH(astar.execute(1, 2), "Graph contains negative edge weights!");
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
    case StepType::PushToQueue:
        return "PushToQueue";
    case StepType::PopFromQueue:
        return "PopFromQueue";
    case StepType::UpdateDistance:
        return "UpdateDistance";
    case StepType::AddToPath:
        return "AddToPath";
    }
    return nullptr;
}

void runAStarLoggingTest(const std::shared_ptr<WeightedDirectedGraph>& g, unsigned startNode,
                         unsigned goalNode) {
    AStar astar(g);
    REQUIRE_NOTHROW(astar.execute(startNode, goalNode));

    const auto& steps = astar.getSteps();

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

    bool hasStart     = false;
    bool hasFinish    = false;
    bool hasVisit     = false;
    bool hasEdge      = false;
    bool hasUpdate    = false;
    bool hasAddToPath = false;
    bool pushToQueue  = false;
    bool popFromQueue = false;

    for(const auto& s: steps) {
        if(s.m_type == StepType::Start) {
            hasStart = true;
        }
        if(s.m_type == StepType::Finish) {
            hasFinish = true;
        }
        if(s.m_type == StepType::VisitNode) {
            hasVisit = true;
        }
        if(s.m_type == StepType::ExamineEdge) {
            hasEdge = true;
        }
        if(s.m_type == StepType::UpdateDistance) {
            hasUpdate = true;
        }
        if(s.m_type == StepType::AddToPath) {
            hasAddToPath = true;
        }
        if(s.m_type == StepType::PushToQueue) {
            pushToQueue = true;
        }
        if(s.m_type == StepType::PopFromQueue) {
            popFromQueue = true;
        }
    }

    REQUIRE(hasStart);
    REQUIRE(hasFinish);
    REQUIRE(hasVisit);
    REQUIRE(hasEdge);
    REQUIRE(hasUpdate);
    REQUIRE(hasAddToPath);
    REQUIRE(pushToQueue);
    REQUIRE(popFromQueue);
}

TEST_CASE("AStar steps test", "[AStar]") {
    auto g = std::make_shared<WeightedDirectedGraph>();

    g->addNode(1, 0, 0);
    g->addNode(2, 1, 0);
    g->addNode(3, 1, 1);
    g->addNode(4, 2, 1);

    g->addEdge(1, 2, 1);
    g->addEdge(2, 4, 2);
    g->addEdge(1, 3, 2);
    g->addEdge(3, 4, 1);

    runAStarLoggingTest(g, 1, 4);
}
