#ifndef ALGORITHMSTEP_H
#define ALGORITHMSTEP_H

#include <optional>
#include <string>
#include <vector>

enum class StepType {
    // opsti koraci
    Start,
    Finish,

    // cvorovi
    VisitNode,
    ProcessNode,
    MarkNode, // Tarjan, Prim

    // grane
    ExamineEdge,
    RelaxEdge,  // Bellman-Ford
    SelectEdge, // Prim

    PushToQueue, // BFS, Kahn, A*
    PopFromQueue,
    PushToStack, // DFS, Tarjan
    PopFromStack,

    UpdateDistance,        // Dijkstra, Bellman-Ford, Floyd-Warshall, A*
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
};

#endif // ALGORITHMSTEP_H
