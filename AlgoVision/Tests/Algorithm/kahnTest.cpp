#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "Kahn.h"
#include "UnweightedDirectedGraph.h"

TEST_CASE("Kahn topological sort on acyclic graph", "[KAHN]") {
    auto graph = std::make_shared<UnweightedDirectedGraph>();

    for(unsigned i = 1; i <= 5; ++i) {
        graph->addNode(i);
    }

    graph->addEdge(1, 2);
    graph->addEdge(1, 3);
    graph->addEdge(3, 4);
    graph->addEdge(2, 4);
    graph->addEdge(4, 5);

    Kahn kahn(graph);
    REQUIRE_NOTHROW(kahn.execute());

    auto sorted = kahn.getSorted();
    REQUIRE(sorted.size() == 5);

    auto pos = std::map<unsigned, unsigned>();
    for(unsigned i = 0; i < sorted.size(); ++i)
        pos[sorted[i]] = i;

    REQUIRE(pos[1] < pos[2]);
    REQUIRE(pos[1] < pos[3]);
    REQUIRE(pos[2] < pos[4]);
    REQUIRE(pos[3] < pos[4]);
    REQUIRE(pos[4] < pos[5]);
}

TEST_CASE("Kahn throws on cyclic graph", "[KAHN]") {
    auto graph = std::make_shared<UnweightedDirectedGraph>();

    graph->addNode(1);
    graph->addNode(2);
    graph->addNode(3);

    graph->addEdge(1, 2);
    graph->addEdge(2, 3);
    graph->addEdge(3, 1);

    Kahn kahn(graph);
    REQUIRE_THROWS_WITH(kahn.execute(), "Graph contains a cycle, topological sort not possible!");
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
    case StepType::AddToTopologicalOrder:
        return "AddToTopologicalOrder";
    }
    return nullptr;
}

void runKahnLoggingTest(const std::shared_ptr<UnweightedDirectedGraph>& g) {
    Kahn kahn(g);
    REQUIRE_NOTHROW(kahn.execute());

    const auto& steps = kahn.getSteps();

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

    bool hasStart        = false;
    bool hasFinish       = false;
    bool hasProcess      = false;
    bool hasPushToQueue  = false;
    bool hasPopFromQueue = false;
    bool hasAddTopOrder  = false;

    for(const auto& s: steps) {
        if(s.m_type == StepType::Start) {
            hasStart = true;
        }
        if(s.m_type == StepType::Finish) {
            hasFinish = true;
        }
        if(s.m_type == StepType::ProcessNode) {
            hasProcess = true;
        }
        if(s.m_type == StepType::PushToQueue) {
            hasPushToQueue = true;
        }
        if(s.m_type == StepType::PopFromQueue) {
            hasPopFromQueue = true;
        }
        if(s.m_type == StepType::AddToTopologicalOrder) {
            hasAddTopOrder = true;
        }
    }

    REQUIRE(hasStart);
    REQUIRE(hasFinish);
    REQUIRE(hasProcess);
    REQUIRE(hasPushToQueue);
    REQUIRE(hasPopFromQueue);
    REQUIRE(hasAddTopOrder);
}

TEST_CASE("Kahn steps test", "[KAHN]") {
    auto graph = std::make_shared<UnweightedDirectedGraph>();

    for(unsigned i = 1; i <= 5; ++i) {
        graph->addNode(i);
    }

    graph->addEdge(1, 2);
    graph->addEdge(1, 3);
    graph->addEdge(3, 4);
    graph->addEdge(2, 4);
    graph->addEdge(4, 5);

    runKahnLoggingTest(graph);
}
