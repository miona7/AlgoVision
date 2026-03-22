#ifndef ALGORITHMSTEP_H
#define ALGORITHMSTEP_H

/**
 * @file AlgorithmStep.h
 * @brief Defines structures and enums for representing and visualizing steps of graph algorithms.
 *
 * This file provides the AlgorithmStep struct and StepType enum, which are used
 * to record changes to nodes and edges during algorithm execution for visualization purposes.
 */

#include <string>
#include <vector>
#include <optional>

#include "Edge.h"
#include "Node.h"

/**
 * @enum StepType
 * @brief Enumerates the types of steps in an algorithm for visualization.
 *
 * Each step represents a meaningful action on a node or edge during
 * algorithm execution.
 */

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

/**
 * @struct AlgorithmStep
 * @brief Represents a single step of an algorithm for visualization.
 *
 * Contains the type of the step, optionally a node or edge affected,
 * and optionally the previous states of nodes/edges before the step.
 */

struct AlgorithmStep {
    StepType m_type{StepType::DefaultType};

    std::optional<unsigned> m_node;
    std::optional<unsigned> m_from;
    std::optional<unsigned> m_to;

    std::optional<NodeState> m_prevNodeState;
    std::optional<EdgeState> m_prevEdgeState;
};

#endif // ALGORITHMSTEP_H
