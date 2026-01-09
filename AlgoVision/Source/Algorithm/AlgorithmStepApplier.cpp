#include "AlgorithmStepApplier.h"

AlgorithmStepApplier::AlgorithmStepApplier(const std::shared_ptr<Graph>& g) : m_graph(g) {
}

void AlgorithmStepApplier::apply(const AlgorithmStep& step) {
    switch(step.m_type) {
    case StepType::VisitNode:
        m_graph->getNode(step.m_node.value())->setState(NodeState::Visited);
        break;
    case StepType::ProcessNode:
        m_graph->getNode(step.m_node.value())->setState(NodeState::Active);
        break;
    case StepType::ExamineEdge:
        m_graph->getEdge(step.m_from.value(), step.m_to.value())->setState(EdgeState::Examined);
        break;
    case StepType::RelaxEdge:
        m_graph->getEdge(step.m_from.value(), step.m_to.value())->setState(EdgeState::Relaxed);
        break;
    case StepType::SelectEdge:
        m_graph->getEdge(step.m_from.value(), step.m_to.value())->setState(EdgeState::Selected);
        break;
    case StepType::AddToPath:
        m_graph->getNode(step.m_node.value())->setState(NodeState::InPath);
        break;
    case StepType::AddToTopologicalOrder:
        m_graph->getNode(step.m_node.value())->setState(NodeState::TopologicalOrder);
        break;
    case StepType::AssignComponent:
        m_graph->getNode(step.m_node.value())->setState(NodeState::AssignedComponent);
        break;
    }
}
