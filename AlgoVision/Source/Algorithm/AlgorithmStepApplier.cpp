#include "AlgorithmStepApplier.h"

AlgorithmStepApplier::AlgorithmStepApplier(const std::shared_ptr<Graph>& g) : m_graph(g) {
}

void AlgorithmStepApplier::apply(const AlgorithmStep& step) {
    check(step);

    switch(step.m_type) {
    case StepType::VisitNode:
        m_graph->getNode(step.m_node.value())->setState(NodeState::Visited);
        break;
    case StepType::ProcessNode:
        m_graph->getNode(step.m_node.value())->setState(NodeState::Active);
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
    case StepType::ExamineEdge:
        m_graph->getEdge(step.m_from.value(), step.m_to.value())->setState(EdgeState::Examined);
        break;
    case StepType::RelaxEdge:
        m_graph->getEdge(step.m_from.value(), step.m_to.value())->setState(EdgeState::Relaxed);
        break;
    case StepType::SelectEdge:
        m_graph->getEdge(step.m_from.value(), step.m_to.value())->setState(EdgeState::Selected);
        break;
    }
}

void AlgorithmStepApplier::check(const AlgorithmStep& step) const {
    switch(step.m_type) {
    case StepType::VisitNode:
    case StepType::ProcessNode:
    case StepType::AddToPath:
    case StepType::AddToTopologicalOrder:
    case StepType::AssignComponent:
        if(!step.m_node.has_value()) {
            throw std::logic_error("Missing Node state!");
        }
        break;

    case StepType::ExamineEdge:
    case StepType::RelaxEdge:
    case StepType::SelectEdge:
        if (!step.m_from.has_value() || !step.m_to.has_value()) {
            throw std::logic_error("Missing Edge state!");
        }
        break;
    }
}
