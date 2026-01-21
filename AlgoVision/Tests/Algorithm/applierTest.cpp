#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "AlgorithmStepApplier.h"
#include "UnweightedDirectedGraph.h"
#include "UnweightedUndirectedGraph.h"
#include "WeightedDirectedGraph.h"
#include "WeightedUndirectedGraph.h"

TEST_CASE("AlgorithmStepApplier works for all graph types", "[AlgorithmStepApplier]") {

    auto testSteps = [](auto graph) {
        // arrange
        AlgorithmStepApplier applier(graph);

        Node* n1 = graph->addNode(0.0, 0.0);
        Node* n2 = graph->addNode(100.0, 0.0);

        graph->addEdge(n1->getId(), n2->getId(), 1);
        Edge* e = graph->getEdge(n1->getId(), n2->getId());

        std::vector<StepType> nodeSteps = { StepType::VisitNode, StepType::ProcessNode, StepType::AddToPath,
                                           StepType::AddToTopologicalOrder, StepType::AssignComponent };

        std::vector<StepType> edgeSteps = { StepType::ExamineEdge, StepType::RelaxEdge, StepType::SelectEdge };

        for(auto type: nodeSteps) {
            AlgorithmStep step;
            step.m_type = type;
            step.m_node = n1->getId();
            NodeState oldNodeState = n1->getState();

            // act
            applier.apply(step);
            // assert
            REQUIRE(n1->getState() == applier.stepToNodeState(type));
            REQUIRE(step.prevNodeState.value() == oldNodeState);

            // act
            applier.undo(step);
            // assert
            REQUIRE(n1->getState() == oldNodeState);
        }

        for(auto type: edgeSteps) {
            AlgorithmStep step;
            step.m_type = type;
            step.m_from = n1->getId();
            step.m_to   = n2->getId();
            EdgeState oldEdgeState = e->getState();

            // act
            applier.apply(step);
            // assert
            REQUIRE(e->getState() == applier.stepToEdgeState(type));
            REQUIRE(step.prevEdgeState.value() == oldEdgeState);

            // act
            applier.undo(step);
            // assert
            REQUIRE(e->getState() == oldEdgeState);
        }
    };

    SECTION("Unweighted Directed Graph") {
        testSteps(std::make_shared<UnweightedDirectedGraph>());
    }
    SECTION("Unweighted Undirected Graph") {
        testSteps(std::make_shared<UnweightedUndirectedGraph>());
    }
    SECTION("Weighted Directed Graph") {
        testSteps(std::make_shared<WeightedDirectedGraph>());
    }
    SECTION("Weighted Undirected Graph") {
        testSteps(std::make_shared<WeightedUndirectedGraph>());
    }
}
