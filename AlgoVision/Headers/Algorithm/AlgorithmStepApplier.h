#ifndef ALGORITHMSTEPAPPLIER_H
#define ALGORITHMSTEPAPPLIER_H

#include <exception>
#include <memory>

#include "AlgorithmStep.h"
#include "Graph.h"

/**
 * @class AlgorithmStepApplier
 * @brief Applies algorithm steps to the graph.
 *
 * This class takes individual algorithm steps and maps them to
 * changes in the state of nodes and edges in the graph.
 */

class AlgorithmStepApplier {
public:
    explicit AlgorithmStepApplier(const std::shared_ptr<Graph>);

    void apply(AlgorithmStep&);
    void undo(const AlgorithmStep&);

    NodeState stepToNodeState(const StepType) const;
    EdgeState stepToEdgeState(const StepType) const;

    void setGraph(const std::shared_ptr<Graph>);

    void resetGraphState();

private:
    std::shared_ptr<Graph> m_graph;

    void check(const AlgorithmStep&) const;
};

#endif // ALGORITHMSTEPAPPLIER_H
