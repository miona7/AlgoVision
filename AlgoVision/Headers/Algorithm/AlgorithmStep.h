#ifndef ALGORITHMSTEP_H
#define ALGORITHMSTEP_H

#include <optional>
#include <string>
#include <vector>

#include "Edge.h"
#include "Node.h"

enum class StepType {
    DefaultType,

    VisitNode,
    ProcessNode,

    ExamineEdge,
    RelaxEdge,
    SelectEdge,

    UpdateDistance,
    AddToPath,
    AddToTopologicalOrder,
    AssignComponent
};

struct AlgorithmStep {
    StepType m_type {StepType::DefaultType};

    std::optional<unsigned> m_node;
    std::optional<unsigned> m_from;
    std::optional<unsigned> m_to;

    // numericke vrednosti
    std::optional<int> m_value;

    std::optional<std::string> m_message;

    // za undo/redo
    std::optional<NodeState> m_prevNodeState;
    std::optional<EdgeState> m_prevEdgeState;
};

#endif // ALGORITHMSTEP_H
