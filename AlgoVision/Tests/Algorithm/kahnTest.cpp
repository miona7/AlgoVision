#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "Kahn.h"
#include "UnweightedDirectedGraph.h"

TEST_CASE("Kahn topological sort on acyclic graph", "[KAHN]") {
    // arrange
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
    int  expectedSize = 5;

    // act
    REQUIRE_NOTHROW(kahn.execute());

    auto sorted = kahn.getSorted();

    // assert
    REQUIRE(sorted.size() == expectedSize);

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
    // arrange
    auto graph = std::make_shared<UnweightedDirectedGraph>();

    graph->addNode(1);
    graph->addNode(2);
    graph->addNode(3);

    graph->addEdge(1, 2);
    graph->addEdge(2, 3);
    graph->addEdge(3, 1);

    Kahn kahn(graph);

    // act + assert
    REQUIRE_THROWS_WITH(kahn.execute(), "Graph contains a cycle, topological sort not possible!");
}

static const char* stepTypeToString(StepType t) {
    switch(t) {
    case StepType::ProcessNode:
        return "ProcessNode";
    case StepType::AddToTopologicalOrder:
        return "AddToTopologicalOrder";
    }
    return nullptr;
}

void runKahnLoggingTest(const std::shared_ptr<UnweightedDirectedGraph> g) {
    // arrange
    Kahn                        kahn(g);
    const std::vector<StepType> expectedSteps = {StepType::ProcessNode,
                                                 StepType::AddToTopologicalOrder};

    // act
    REQUIRE_NOTHROW(kahn.execute());

    const auto& steps = kahn.getSteps();
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

TEST_CASE("Kahn steps test", "[KAHN]") {
    // arrange
    auto graph = std::make_shared<UnweightedDirectedGraph>();

    for(unsigned i = 1; i <= 5; ++i) {
        graph->addNode(i);
    }

    graph->addEdge(1, 2);
    graph->addEdge(1, 3);
    graph->addEdge(3, 4);
    graph->addEdge(2, 4);
    graph->addEdge(4, 5);

    // act + assert
    runKahnLoggingTest(graph);
}
