#ifndef ASA_H
#define ASA_H

#include <exception>

#include "AlgorithmStep.h"
#include "Graph.h"

class AlgorithmStepApplier {
public:
    explicit AlgorithmStepApplier(const std::shared_ptr<Graph>);

    void apply(AlgorithmStep&);
    void undo(const AlgorithmStep&);

    NodeState stepToNodeState(const StepType) const;
    EdgeState stepToEdgeState(const StepType) const;

private:
    std::shared_ptr<Graph> m_graph;

    void check(const AlgorithmStep&) const;
};

#endif // ASA_H
