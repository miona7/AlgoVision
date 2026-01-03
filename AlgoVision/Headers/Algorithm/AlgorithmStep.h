#ifndef ALGORITHMSTEP_H
#define ALGORITHMSTEP_H

#include <string>
#include <vector>
#include <optional>

enum class StepType {
    // opsti koraci
    Start,
    Finish,

    // cvorovi
    VisitNode,    // DFS, BFS, Dijkstra, A*
    ProcessNode,
    MarkNode,     // Tarjan, Prim

    // grane
    ExamineEdge,
    RelaxEdge,    // BellmanFord
    SelectEdge,   // Prim

    PushToQueue,  // BFS, Kahn
    PopFromQueue,
    PushToStack,  // DFS, Tarjan
    PopFromStack,

    UpdateDistance,  // Dijkstra, Bellman-Ford, Floyd-Warshall
    AddToPath,       // A*
    AddToTopologicalOrder,
    AssignComponent  // Tarjan
};

struct AlgorithmStep {
    StepType m_type;

    std::optional<unsigned> m_node;
    std::optional<unsigned> m_edge;

    // numericke vrednosti
    std::optional<int> m_value;

    std::optional<std::string> m_message;
};

#endif // ALGORITHMSTEP_H
