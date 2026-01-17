#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "Dijkstra.h"
#include "UnweightedDirectedGraph.h"
#include "UnweightedUndirectedGraph.h"
#include "WeightedDirectedGraph.h"
#include "WeightedUndirectedGraph.h"

TEST_CASE("Dijkstra on Unweighted Directed Graph", "[DIJKSTRA]") {
    auto udg = std::make_shared<UnweightedDirectedGraph>();
    for(unsigned i = 1; i <= 5; ++i) {
        udg->addNode(i);
    }

    udg->addEdge(1, 2);
    udg->addEdge(1, 3);
    udg->addEdge(2, 4);
    udg->addEdge(3, 4);
    udg->addEdge(4, 5);

    Dijkstra dijkstra(udg);
    REQUIRE_NOTHROW(dijkstra.execute(1));
}

TEST_CASE("Dijkstra on Unweighted Undirected Graph", "[DIJKSTRA]") {
    auto uug = std::make_shared<UnweightedUndirectedGraph>();
    for(unsigned i = 1; i <= 5; ++i) {
        uug->addNode(i);
    }

    uug->addEdge(1, 2);
    uug->addEdge(1, 3);
    uug->addEdge(2, 4);
    uug->addEdge(3, 4);
    uug->addEdge(4, 5);

    Dijkstra dijkstra(uug);
    REQUIRE_NOTHROW(dijkstra.execute(1));
}

TEST_CASE("Dijkstra on Weighted Directed Graph", "[DIJKSTRA]") {
    auto wdg = std::make_shared<WeightedDirectedGraph>();
    for(unsigned i = 1; i <= 6; ++i) {
        wdg->addNode(i);
    }

    wdg->addEdge(1, 2, 2);
    wdg->addEdge(1, 3, 5);
    wdg->addEdge(2, 4, 1);
    wdg->addEdge(3, 4, 2);
    wdg->addEdge(4, 5, 1);
    wdg->addEdge(5, 6, 3);

    Dijkstra dijkstra(wdg);
    REQUIRE_NOTHROW(dijkstra.execute(1));
}

TEST_CASE("Dijkstra on Weighted Undirected Graph", "[DIJKSTRA]") {
    auto wug = std::make_shared<WeightedUndirectedGraph>();
    for(unsigned i = 1; i <= 5; ++i) {
        wug->addNode(i);
    }

    wug->addEdge(1, 2, 2);
    wug->addEdge(1, 3, 4);
    wug->addEdge(2, 4, 1);
    wug->addEdge(3, 4, 3);
    wug->addEdge(4, 5, 5);

    Dijkstra dijkstra(wug);
    REQUIRE_NOTHROW(dijkstra.execute(1));
}

TEST_CASE("Dijkstra with invalid start node", "[DIJKSTRA]") {
    auto udg = std::make_shared<UnweightedDirectedGraph>();
    udg->addNode(1);
    udg->addNode(2);

    Dijkstra dijkstra(udg);
    REQUIRE_THROWS_AS(dijkstra.execute(0), std::runtime_error);
}

TEST_CASE("Dijkstra on graph with negative edges", "[DIJKSTRA]") {
    auto wdg = std::make_shared<WeightedDirectedGraph>();
    for(unsigned i = 1; i <= 3; ++i) {
        wdg->addNode(i);
    }

    wdg->addEdge(1, 2, -5);
    wdg->addEdge(2, 3, 2);

    Dijkstra dijkstra(wdg);
    REQUIRE_THROWS_AS(dijkstra.execute(1), std::runtime_error);
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
    }
    return nullptr;
}

void runDijkstraLoggingTest(const std::shared_ptr<Graph>& g, unsigned startNode) {
    // arrange
    Dijkstra dijkstra(g);
    const std::vector<StepType> expectedSteps = {
        StepType::Start, StepType::Finish, StepType::VisitNode,
        StepType::ProcessNode, StepType::ExamineEdge, StepType::UpdateDistance,
        StepType::PushToQueue, StepType::PopFromQueue
    };

    // act
    REQUIRE_NOTHROW(dijkstra.execute(startNode));

    const auto& steps = dijkstra.getSteps();
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

    for(auto expected : expectedSteps) {
        bool found = std::any_of(steps.begin(), steps.end(), [&](const auto& s){ return s.m_type == expected; });
        REQUIRE(found);
    }
}

TEST_CASE("Dijkstra steps test on all graph types", "[DIJKSTRA]") {

    SECTION("Unweighted Directed Graph") {
        auto udg = std::make_shared<UnweightedDirectedGraph>();

        for(unsigned i = 1; i <= 5; ++i) {
            udg->addNode(i);
        }

        udg->addEdge(1, 2);
        udg->addEdge(1, 3);
        udg->addEdge(2, 4);
        udg->addEdge(3, 4);
        udg->addEdge(4, 5);

        runDijkstraLoggingTest(udg, 1);
    }

    SECTION("Unweighted Unirected Graph") {
        auto uug = std::make_shared<UnweightedUndirectedGraph>();

        for(unsigned i = 1; i <= 5; ++i) {
            uug->addNode(i);
        }

        uug->addEdge(1, 2);
        uug->addEdge(1, 3);
        uug->addEdge(2, 4);
        uug->addEdge(3, 4);
        uug->addEdge(4, 5);

        runDijkstraLoggingTest(uug, 1);
    }

    SECTION("Weighted Directed Graph") {
        auto wdg = std::make_shared<WeightedDirectedGraph>();

        for(unsigned i = 1; i <= 6; ++i) {
            wdg->addNode(i);
        }

        wdg->addEdge(1, 2, 2);
        wdg->addEdge(1, 3, 5);
        wdg->addEdge(2, 4, 1);
        wdg->addEdge(3, 4, 2);
        wdg->addEdge(4, 5, 1);
        wdg->addEdge(5, 6, 3);

        runDijkstraLoggingTest(wdg, 1);
    }

    SECTION("Weighted Undirected Graph") {
        auto wug = std::make_shared<WeightedUndirectedGraph>();

        for(unsigned i = 1; i <= 5; ++i) {
            wug->addNode(i);
        }

        wug->addEdge(1, 2, 2);
        wug->addEdge(1, 3, 4);
        wug->addEdge(2, 4, 1);
        wug->addEdge(3, 4, 3);
        wug->addEdge(4, 5, 5);

        runDijkstraLoggingTest(wug, 1);
    }
}
