#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "AlgorithmStep.h"
#include "Tarjan.h"
#include "UnweightedDirectedGraph.h"
#include "UnweightedUndirectedGraph.h"
#include "WeightedDirectedGraph.h"

#include <iostream>

static const char* stepTypeToString(StepType t) {
    switch(t) {
    case StepType::Start:
        return "Start";
    case StepType::Finish:
        return "Finish";
    case StepType::VisitNode:
        return "VisitNode";
    case StepType::ExamineEdge:
        return "ExamineEdge";
    case StepType::PushToStack:
        return "PushToStack";
    case StepType::PopFromStack:
        return "PopFromStack";
    case StepType::AssignComponent:
        return "AssignComponent";
    case StepType::UpdateDistance:
        return "UpdateDistance";
    default:
        return "Other";
    }
}

static void printSteps(const std::vector<AlgorithmStep>& steps) {
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
}

static void verifyTarjanTrace(const std::vector<AlgorithmStep>& steps, std::size_t expectedNodes) {
    bool hasStart     = false;
    bool hasFinish    = false;
    int  visitCount   = 0;
    int  assignCount  = 0;
    int  pushCount    = 0;
    int  popCount     = 0;
    int  examineCount = 0;

    for(const auto& s: steps) {
        if(s.m_type == StepType::Start)
            hasStart = true;
        if(s.m_type == StepType::Finish)
            hasFinish = true;
        if(s.m_type == StepType::VisitNode)
            ++visitCount;
        if(s.m_type == StepType::AssignComponent)
            ++assignCount;
        if(s.m_type == StepType::PushToStack)
            ++pushCount;
        if(s.m_type == StepType::PopFromStack)
            ++popCount;
        if(s.m_type == StepType::ExamineEdge)
            ++examineCount;
    }

    std::cout << "Start present:            " << (hasStart ? "YES" : "NO") << "\n";
    std::cout << "Finish present:           " << (hasFinish ? "YES" : "NO") << "\n";
    std::cout << "VisitNode steps:          " << visitCount << " (expected " << expectedNodes
              << ")\n";
    std::cout << "AssignComponent steps:    " << assignCount << " (expected " << expectedNodes
              << ")\n";
    std::cout << "PushToStack steps:        " << pushCount << " (expected " << expectedNodes
              << ")\n";
    std::cout << "PopFromStack steps:       " << popCount << " (expected " << expectedNodes
              << ")\n";
    std::cout << "ExamineEdge steps:        " << examineCount << " (expected > 0)\n";

    bool ok = hasStart && hasFinish && visitCount == (int)expectedNodes &&
              assignCount == (int)expectedNodes && pushCount == (int)expectedNodes &&
              popCount == (int)expectedNodes && examineCount > 0;

    if(ok) {
        std::cout << "Tarjan AlgorithmSteps are working correctly.\n";
    } else {
        std::cout << "Tarjan AlgorithmSteps are incomplete or invalid.\n";
    }

    REQUIRE(hasStart);
    REQUIRE(hasFinish);
    REQUIRE(visitCount == (int)expectedNodes);
    REQUIRE(assignCount == (int)expectedNodes);
    REQUIRE(pushCount == (int)expectedNodes);
    REQUIRE(popCount == (int)expectedNodes);
    REQUIRE(examineCount > 0);

    std::cout << "=================================================\n\n";
}

TEST_CASE("Tarjan on Unweighted Directed Graph produces step trace", "[TARJAN][Steps][Debug]") {
    std::cout << "\n================ TARJAN STEPS TEST (Unweighted) ================\n";

    auto udg = std::make_shared<UnweightedDirectedGraph>();
    for(unsigned i = 1; i <= 5; ++i) {
        udg->addNode(i);
    }

    udg->addEdge(1, 2);
    udg->addEdge(2, 3);
    udg->addEdge(3, 1);
    udg->addEdge(3, 4);
    udg->addEdge(4, 5);

    Tarjan tarjan(udg);
    tarjan.execute();

    const auto& steps = tarjan.getSteps();
    REQUIRE_FALSE(steps.empty());

    printSteps(steps);
    verifyTarjanTrace(steps, udg->getNodes().size());

    std::cout << "=============================================================\n\n";
}

TEST_CASE("Tarjan on Weighted Directed Graph produces step trace", "[TARJAN][Steps][Debug]") {
    std::cout << "\n================ TARJAN STEPS TEST (Weighted) ================\n";

    auto wdg = std::make_shared<WeightedDirectedGraph>();
    for(unsigned i = 1; i <= 6; ++i) {
        wdg->addNode(i);
    }

    wdg->addEdge(1, 2, 1);
    wdg->addEdge(2, 3, 2);
    wdg->addEdge(3, 1, 3);
    wdg->addEdge(3, 4, 1);
    wdg->addEdge(4, 5, 2);
    wdg->addEdge(5, 6, 1);
    wdg->addEdge(6, 4, 3);

    Tarjan tarjan(wdg);
    tarjan.execute();

    const auto& steps = tarjan.getSteps();
    REQUIRE_FALSE(steps.empty());

    printSteps(steps);
    verifyTarjanTrace(steps, wdg->getNodes().size());

    std::cout << "=============================================================\n\n";
}

TEST_CASE("Tarjan fails on Undirected Graph", "[TARJAN]") {
    auto uug = std::make_shared<UnweightedUndirectedGraph>();
    for(unsigned i = 1; i <= 4; ++i) {
        uug->addNode(i);
    }

    uug->addEdge(1, 2);
    uug->addEdge(2, 3);
    uug->addEdge(3, 4);

    Tarjan tarjan(uug);
    REQUIRE_THROWS_AS(tarjan.execute(), std::runtime_error);
}
