#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "AlgorithmStep.h"
#include "BellmanFord.h"
#include "WeightedDirectedGraph.h"
#include "WeightedUndirectedGraph.h"

#include <iostream>

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
    case StepType::ExamineEdge:
        return "ExamineEdge";
    case StepType::RelaxEdge:
        return "RelaxEdge";
    case StepType::UpdateDistance:
        return "UpdateDistance";
    case StepType::MarkNode:
        return "MarkNode";
    default:
        return "Other";
    }
}

TEST_CASE("Bellman-Ford without negative cycle", "[BF]") {
    // arrange
    auto graph = std::make_shared<WeightedDirectedGraph>();

    for(unsigned i = 1; i <= 5; ++i) {
        graph->addNode(i);
    }

    graph->addEdge(1, 2, 6);
    graph->addEdge(1, 3, 7);
    graph->addEdge(2, 4, 5);
    graph->addEdge(3, 4, -3);
    graph->addEdge(4, 5, 2);

    BellmanFord bf(graph);

    // act
    REQUIRE_NOTHROW(bf.execute(1));

    bool negativeCycle = bf.hasNegativeCycle();

    // assert
    REQUIRE_FALSE(negativeCycle);
}

TEST_CASE("Bellman-Ford with negative cycle", "[BF]") {
    // arrange
    auto graph = std::make_shared<WeightedDirectedGraph>();

    for(unsigned i = 1; i <= 3; ++i) {
        graph->addNode(i);
    }

    graph->addEdge(1, 2, 1);
    graph->addEdge(2, 3, -2);
    graph->addEdge(3, 1, -2);

    BellmanFord bf(graph);

    // act
    REQUIRE_NOTHROW(bf.execute(1));

    bool negativeCycle = bf.hasNegativeCycle();

    // assert
    REQUIRE(negativeCycle);
}

TEST_CASE("Bellman-Ford on invalid graph", "[BF]") {
    // arrange
    auto wud = std::make_shared<WeightedUndirectedGraph>();

    wud->addNode(1);
    wud->addNode(2);
    wud->addEdge(1, 2, 6);

    BellmanFord bf(wud);

    // act + assert
    REQUIRE_THROWS_AS(bf.execute(1), std::runtime_error);
}

TEST_CASE("Bellman-Ford: start node does not exist", "[BF]") {
    // arrange
    auto graph = std::make_shared<WeightedDirectedGraph>();

    for(unsigned i = 1; i <= 2; ++i) {
        graph->addNode(i);
    }

    BellmanFord bf(graph);

    // act + assert
    REQUIRE_THROWS_AS(bf.execute(0), std::runtime_error);
}

TEST_CASE("Bellman-Ford: empty graph", "[BF]") {
    // arrange
    auto graph = std::make_shared<WeightedDirectedGraph>();

    BellmanFord bf(graph);

    // act + assert
    REQUIRE_THROWS_AS(bf.execute(0), std::runtime_error);
}

TEST_CASE("Bellman-Ford: graph with disconnected nodes", "[BF]") {
    // arrange
    auto graph = std::make_shared<WeightedDirectedGraph>();

    for(unsigned i = 1; i <= 4; ++i) {
        graph->addNode(i);
    }

    graph->addEdge(1, 2, 3);
    graph->addEdge(2, 3, 2);

    BellmanFord bf(graph);

    // act + assert
    REQUIRE_NOTHROW(bf.execute(1));
}

TEST_CASE("Bellman-Ford produces and logs AlgorithmSteps (manual verification)",
          "[BF][Steps][Debug]") {
    std::cout << "\n================ BELLMAN-FORD STEPS TEST ================\n";

    auto graph = std::make_shared<WeightedDirectedGraph>();
    for(unsigned i = 1; i <= 5; ++i) {
        graph->addNode(i);
    }

    graph->addEdge(1, 2, 6);
    graph->addEdge(1, 3, 7);
    graph->addEdge(2, 4, 5);
    graph->addEdge(3, 4, -3);
    graph->addEdge(4, 5, 2);

    BellmanFord bf(graph);
    bf.execute(1);

    const auto& steps = bf.getSteps();
    REQUIRE_FALSE(steps.empty());

    std::cout << "Total steps produced: " << steps.size() << "\n\n";

    for(std::size_t i = 0; i < steps.size(); ++i) {
        const auto& s = steps[i];

        std::cout << "[" << i << "] " << stepTypeToString(s.m_type);

        if(s.m_node) {
            std::cout << " | node=" << *s.m_node;
        }
        if(s.m_from && s.m_to) {
            std::cout << " | edge=" << *s.m_from << "->" << *s.m_to;
        }
        if(s.m_value) {
            std::cout << " | value=" << *s.m_value;
        }
        if(s.m_message) {
            std::cout << " | msg=\"" << *s.m_message << "\"";
        }

        std::cout << "\n";
    }

    bool hasStart     = false;
    bool hasFinish    = false;
    int  examineCount = 0;
    int  relaxCount   = 0;
    bool negCycleMsg  = false;

    for(const auto& s: steps) {
        if(s.m_type == StepType::Start)
            hasStart = true;
        if(s.m_type == StepType::Finish)
            hasFinish = true;
        if(s.m_type == StepType::ExamineEdge)
            ++examineCount;
        if(s.m_type == StepType::RelaxEdge)
            ++relaxCount;
        if(s.m_type == StepType::MarkNode && s.m_message &&
           s.m_message->find("negative cycle") != std::string::npos) {
            negCycleMsg = true;
        }
    }

    std::cout << "Start step present:          " << (hasStart ? "YES" : "NO") << "\n";
    std::cout << "Finish step present:         " << (hasFinish ? "YES" : "NO") << "\n";
    std::cout << "ExamineEdge steps:           " << examineCount << "\n";
    std::cout << "RelaxEdge steps:             " << relaxCount << "\n";
    std::cout << "Negative-cycle message step: " << (negCycleMsg ? "YES" : "NO") << "\n";
    std::cout << "hasNegativeCycle() flag:     " << (bf.hasNegativeCycle() ? "TRUE" : "FALSE")
              << "\n";

    if(hasStart && hasFinish && examineCount > 0 && relaxCount > 0 && !bf.hasNegativeCycle()) {
        std::cout << "Bellman-Ford AlgorithmSteps are working correctly.\n";
    } else {
        std::cout << "Bellman-Ford AlgorithmSteps are incomplete or invalid.\n";
    }

    REQUIRE(hasStart);
    REQUIRE(hasFinish);
    REQUIRE(examineCount > 0);
    REQUIRE(relaxCount > 0);
    REQUIRE_FALSE(bf.hasNegativeCycle());
    REQUIRE_FALSE(negCycleMsg);

    std::cout << "=============================================================\n\n";
}

TEST_CASE("Bellman-Ford detects negative cycle (flag + optional step)", "[BF][NegCycle]") {
    std::cout << "\n================ BELLMAN-FORD NEGATIVE CYCLE TEST ================\n";

    auto graph = std::make_shared<WeightedDirectedGraph>();
    for(unsigned i = 1; i <= 3; ++i) {
        graph->addNode(i);
    }

    graph->addEdge(1, 2, 1);
    graph->addEdge(2, 3, -2);
    graph->addEdge(3, 1, -2);

    BellmanFord bf(graph);
    bf.execute(1);

    const auto& steps = bf.getSteps();
    REQUIRE_FALSE(steps.empty());

    bool hasStart    = false;
    bool hasFinish   = false;
    bool negCycleMsg = false;

    for(const auto& s: steps) {
        if(s.m_type == StepType::Start)
            hasStart = true;
        if(s.m_type == StepType::Finish)
            hasFinish = true;
        if(s.m_type == StepType::MarkNode && s.m_message &&
           s.m_message->find("negative cycle") != std::string::npos) {
            negCycleMsg = true;
        }
    }

    std::cout << "Start: " << (hasStart ? "YES" : "NO") << "\n";
    std::cout << "Finish: " << (hasFinish ? "YES" : "NO") << "\n";
    std::cout << "Negative-cycle message step: " << (negCycleMsg ? "YES" : "NO") << "\n";
    std::cout << "hasNegativeCycle() flag: " << (bf.hasNegativeCycle() ? "TRUE" : "FALSE") << "\n";

    if(hasStart && hasFinish && bf.hasNegativeCycle()) {
        std::cout << "Negative cycle detection is working.\n";
    } else {
        std::cout << "Negative cycle detection is not working.\n";
    }

    REQUIRE(hasStart);
    REQUIRE(hasFinish);
    REQUIRE(bf.hasNegativeCycle());

    std::cout << "==================================================================\n\n";
}
