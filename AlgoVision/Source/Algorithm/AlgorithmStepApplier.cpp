#include "AlgorithmStepApplier.h"

AlgorithmStepApplier::AlgorithmStepApplier(const std::shared_ptr<Graph> g) : m_graph(g) {
}

void AlgorithmStepApplier::apply(AlgorithmStep& step) {
    check(step);

    switch(step.m_type) {
    case StepType::VisitNode:
    case StepType::ProcessNode:
    case StepType::UpdateDistance:
    case StepType::AddToPath:
    case StepType::AddToTopologicalOrder:
    case StepType::AssignComponent: {
        Node* node = m_graph->getNode(step.m_node.value());
        step.m_prevNodeState = node->getState();      // snimi staro stanje
        node->setState(stepToNodeState(step.m_type)); // promeni stanje
        break;
    }
    case StepType::ExamineEdge:
    case StepType::RelaxEdge:
    case StepType::SelectEdge: {
        Edge* edge = m_graph->getEdge(step.m_from.value(), step.m_to.value());
        step.m_prevEdgeState = edge->getState();      // snimi staro stanje
        edge->setState(stepToEdgeState(step.m_type)); // promeni stanje
        break;
    }
    default:
        break;
    }
}

void AlgorithmStepApplier::undo(const AlgorithmStep& step) {
    switch(step.m_type) {
    case StepType::VisitNode:
    case StepType::ProcessNode:
    case StepType::UpdateDistance:
    case StepType::AddToPath:
    case StepType::AddToTopologicalOrder:
    case StepType::AssignComponent:
        if(step.m_node && step.m_prevNodeState.has_value()) {
            m_graph->getNode(step.m_node.value())->setState(step.m_prevNodeState.value());
        }
        break;
    case StepType::ExamineEdge:
    case StepType::RelaxEdge:
    case StepType::SelectEdge:
        if(step.m_from && step.m_to && step.m_prevEdgeState.has_value()) {
            m_graph->getEdge(step.m_from.value(), step.m_to.value())->setState(step.m_prevEdgeState.value());
        }
        break;
    default:
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
        if(!step.m_from.has_value() || !step.m_to.has_value()) {
            throw std::logic_error("Missing Edge state!");
        }
        break;
    }
}

NodeState AlgorithmStepApplier::stepToNodeState(const StepType t) const {
    switch(t) {
    case StepType::VisitNode:
        return NodeState::Visited;
    case StepType::ProcessNode:
        return NodeState::Active;
    case StepType::UpdateDistance:
        return NodeState::UpdatedDistance;
    case StepType::AddToPath:
        return NodeState::InPath;
    case StepType::AddToTopologicalOrder:
        return NodeState::TopologicalOrder;
    case StepType::AssignComponent:
        return NodeState::AssignedComponent;
    default:
        return NodeState::Default;
    }
}

EdgeState AlgorithmStepApplier::stepToEdgeState(const StepType t) const {
    switch(t) {
    case StepType::ExamineEdge:
        return EdgeState::Examined;
    case StepType::RelaxEdge:
        return EdgeState::Relaxed;
    case StepType::SelectEdge:
        return EdgeState::Selected;
    default:
        return EdgeState::Default;
    }
}

