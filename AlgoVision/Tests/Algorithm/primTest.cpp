#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "AlgorithmStep.h"
#include "Prim.h"
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
    case StepType::MarkNode:
        return "MarkNode";
    case StepType::ExamineEdge:
        return "ExamineEdge";
    case StepType::UpdateDistance:
        return "UpdateDistance";
    case StepType::SelectEdge:
        return "SelectEdge";
    default:
        return "Other";
    }
}

TEST_CASE("Prim works on connected undirected weighted graph", "[PRIM]") {
    auto g = std::make_shared<WeightedUndirectedGraph>();

    g->addNode(1);
    g->addNode(2);
    g->addNode(3);
    g->addNode(4);

    g->addEdge(1, 2, 1);
    g->addEdge(2, 3, 2);
    g->addEdge(3, 4, 3);
    g->addEdge(1, 4, 10);

    Prim prim(g);

    REQUIRE_NOTHROW(prim.execute());
}

TEST_CASE("Prim throws on disconnected graph", "[PRIM]") {
    auto g = std::make_shared<WeightedUndirectedGraph>();

    g->addNode(1);
    g->addNode(2);
    g->addNode(3);

    g->addEdge(1, 2, 1);
    // cvor 3 je izolovan

    Prim prim(g);

    REQUIRE_THROWS_AS(prim.execute(), std::runtime_error);
}

TEST_CASE("Prim throws on directed graph", "[PRIM]") {
    auto g = std::make_shared<WeightedDirectedGraph>();

    g->addNode(1);
    g->addNode(2);

    g->addEdge(1, 2, 5);

    Prim prim(g);

    REQUIRE_THROWS_AS(prim.execute(), std::runtime_error);
}

TEST_CASE("Prim throws on empty graph", "[PRIM]") {
    auto g = std::make_shared<WeightedUndirectedGraph>();

    Prim prim(g);

    REQUIRE_THROWS_AS(prim.execute(), std::runtime_error);
}

TEST_CASE("Prim produces and logs AlgorithmSteps (manual verification)", "[PRIM][Steps][Debug]") {
    std::cout << "\n================ PRIM STEPS TEST ================\n";

    auto g = std::make_shared<WeightedUndirectedGraph>();

    g->addNode(1);
    g->addNode(2);
    g->addNode(3);
    g->addNode(4);

    g->addEdge(1, 2, 1);
    g->addEdge(2, 3, 2);
    g->addEdge(3, 4, 3);
    g->addEdge(1, 4, 10);

    Prim prim(g);
    prim.execute();

    const auto& steps = prim.getSteps();
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

    bool hasStart  = false;
    bool hasFinish = false;
    int  mstEdges  = 0;

    for(const auto& s: steps) {
        if(s.m_type == StepType::Start)
            hasStart = true;
        if(s.m_type == StepType::Finish)
            hasFinish = true;
        if(s.m_type == StepType::SelectEdge)
            ++mstEdges;
    }

    std::cout << "Start step present:  " << (hasStart ? "YES" : "NO") << "\n";
    std::cout << "Finish step present: " << (hasFinish ? "YES" : "NO") << "\n";
    std::cout << "MST edges selected:  " << mstEdges << "\n";

    if(hasStart && hasFinish && mstEdges == static_cast<int>(g->getNodes().size()) - 1) {
        std::cout << "Prim AlgorithmSteps are working correctly.\n";
    } else {
        std::cout << " Prim AlgorithmSteps are incomplete or invalid.\n";
    }

    REQUIRE(hasStart);
    REQUIRE(hasFinish);
    REQUIRE(mstEdges == static_cast<int>(g->getNodes().size()) - 1);

    std::cout << "====================================================\n\n";
}
