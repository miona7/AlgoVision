#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "FloydWarshall.h"
#include "AlgorithmStep.h"
#include "WeightedDirectedGraph.h"

#include <iostream>

static const char* stepTypeToString(StepType t)
{
    switch (t) {
    case StepType::Start: return "Start";
    case StepType::Finish: return "Finish";
    case StepType::ProcessNode: return "ProcessNode";
    case StepType::ExamineEdge: return "ExamineEdge";
    case StepType::UpdateDistance: return "UpdateDistance";
    default: return "Other";
    }
}

TEST_CASE("Floyd-Warshall produces and logs AlgorithmSteps (manual verification)", "[FW][Steps][Debug]")
{
    std::cout << "\n================ FLOYD-WARSHALL STEPS TEST ================\n";

    auto graph = std::make_shared<WeightedDirectedGraph>();
    for (unsigned i = 1; i <= 4; ++i) {
        graph->addNode(i);
    }

    graph->addEdge(1, 2, 3);
    graph->addEdge(2, 3, 2);
    graph->addEdge(3, 4, 4);
    graph->addEdge(1, 4, 10);
    // Ovde postoji poboljšanje: 1->4 može preko 2,3: 3+2+4=9 < 10

    FloydWarshall fw(graph);
    fw.execute();

    const auto& steps = fw.getSteps();
    REQUIRE_FALSE(steps.empty());

    std::cout << "Total steps produced: " << steps.size() << "\n\n";

    for (std::size_t i = 0; i < steps.size(); ++i) {
        const auto& s = steps[i];

        std::cout << "[" << i << "] " << stepTypeToString(s.m_type);

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

    bool hasStart = false;
    bool hasFinish = false;
    int  processKCount = 0;
    int  updateCount = 0;
    bool hasImprovement_1_to_4 = false;

    for (const auto& s : steps) {
        if (s.m_type == StepType::Start) hasStart = true;
        if (s.m_type == StepType::Finish) hasFinish = true;
        if (s.m_type == StepType::ProcessNode) ++processKCount;
        if (s.m_type == StepType::UpdateDistance) {
            ++updateCount;

            if (s.m_from && s.m_to && s.m_value) {
                if (*s.m_from == 1 && *s.m_to == 4 && *s.m_value == 9) {
                    hasImprovement_1_to_4 = true;
                }
            }
        }
    }

    std::cout << "Start step present:         " << (hasStart ? "YES" : "NO") << "\n";
    std::cout << "Finish step present:        " << (hasFinish ? "YES" : "NO") << "\n";
    std::cout << "ProcessNode (k) steps:      " << processKCount
              << " (expected >= " << graph->getNodes().size() << ")\n";
    std::cout << "UpdateDistance steps:       " << updateCount << "\n";
    std::cout << "Improvement 1->4 to 9 seen: " << (hasImprovement_1_to_4 ? "YES" : "NO") << "\n";

    if (hasStart && hasFinish && processKCount >= (int)graph->getNodes().size() && updateCount > 0) {
        std::cout << "Floyd-Warshall AlgorithmSteps are WORKING correctly.\n";
    } else {
        std::cout << "Floyd-Warshall AlgorithmSteps are INCOMPLETE or INVALID.\n";
    }

    REQUIRE(hasStart);
    REQUIRE(hasFinish);
    REQUIRE(processKCount >= (int)graph->getNodes().size());
    REQUIRE(updateCount > 0);
    REQUIRE(hasImprovement_1_to_4);

    std::cout << "================================================================\n\n";
}

TEST_CASE("Floyd-Warshall: graph with negative cycle throws", "[FW]")
{
    auto graph = std::make_shared<WeightedDirectedGraph>();
    for (unsigned i = 1; i <= 3; ++i) {
        graph->addNode(i);
    }

    graph->addEdge(1, 2, 1);
    graph->addEdge(2, 3, -2);
    graph->addEdge(3, 1, -2);

    FloydWarshall fw(graph);

    REQUIRE_THROWS_AS(fw.execute(), std::runtime_error);
}
