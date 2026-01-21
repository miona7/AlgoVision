#ifndef ALGORITHMSTEP_H
#define ALGORITHMSTEP_H

#include <optional>
#include <string>
#include <vector>

#include "Node.h"
#include "Edge.h"

enum class StepType {
    // opsti koraci -> TODO: izbaciti
    Start,
    Finish,

    // cvorovi
    VisitNode,
    ProcessNode,
    MarkNode, // Tarjan, Prim TODO: izbaciti

    // grane
    ExamineEdge,
    RelaxEdge,  // Bellman-Ford
    SelectEdge, // Prim

    // TODO: izbaciti sva 4
    PushToQueue, // BFS, Kahn, A*
    PopFromQueue,
    PushToStack, // DFS, Tarjan
    PopFromStack,

    UpdateDistance,        // Dijkstra, Bellman-Ford, Floyd-Warshall, A*, (izbaciti?)
    AddToPath,             // A*
    AddToTopologicalOrder, // Kahn
    AssignComponent        // Tarjan
};

struct AlgorithmStep {
    StepType m_type {StepType::Start};

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
