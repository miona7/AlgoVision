#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "BFS.h"
#include "AlgorithmStep.h"
#include "UnweightedDirectedGraph.h"

#include <iostream>
#include <sstream>

static const char* stepTypeToString(StepType t)
{
    switch (t) {
    case StepType::Start: return "Start";
    case StepType::Finish: return "Finish";
    case StepType::VisitNode: return "VisitNode";
    case StepType::ProcessNode: return "ProcessNode";
    case StepType::MarkNode: return "MarkNode";
    case StepType::ExamineEdge: return "ExamineEdge";
    case StepType::PushToQueue: return "PushToQueue";
    case StepType::PopFromQueue: return "PopFromQueue";
    default: return "Other";
    }
}

TEST_CASE("BFS produces and logs AlgorithmSteps (manual verification)", "[BFS][Steps][Debug]")
{
    std::cout << "\n================ BFS STEPS TEST ================\n";

    auto g = std::make_shared<UnweightedDirectedGraph>();
    g->addNode(0);
    g->addNode(1);
    g->addNode(2);

    g->addEdge(0, 1);
    g->addEdge(1, 2);

    BFS bfs(g);
    bfs.execute(0);

    const auto& steps = bfs.getSteps();

    std::cout << "Total steps produced: " << steps.size() << "\n\n";

    REQUIRE_FALSE(steps.empty());

    for (std::size_t i = 0; i < steps.size(); ++i) {
        const auto& s = steps[i];

        std::cout << "[" << i << "] "
                  << stepTypeToString(s.m_type);

        if (s.m_node) {
            std::cout << " | node=" << *s.m_node;
        }
        if (s.m_from && s.m_to) {
            std::cout << " | edge=" << *s.m_from << "->" << *s.m_to;
        }
        if (s.m_value) {
            std::cout << " | value=" << *s.m_value;
        }
        if (s.m_message) {
            std::cout << " | msg=\"" << *s.m_message << "\"";
        }

        std::cout << "\n";
    }

    bool hasStart  = false;
    bool hasFinish = false;
    bool hasVisit  = false;
    bool hasEdge   = false;

    for (const auto& s : steps) {
        if (s.m_type == StepType::Start) hasStart = true;
        if (s.m_type == StepType::Finish) hasFinish = true;
        if (s.m_type == StepType::VisitNode) hasVisit = true;
        if (s.m_type == StepType::ExamineEdge) hasEdge = true;
    }


    if (hasStart && hasFinish && hasVisit && hasEdge) {
        std::cout << "BFS AlgorithmSteps are working correctly.\n";
    } else {
        std::cout << "BFS AlgorithmSteps are incomplete.\n";
    }

    REQUIRE(hasStart);
    REQUIRE(hasFinish);
    REQUIRE(hasVisit);
    REQUIRE(hasEdge);

    std::cout << "=================================================\n\n";
}
