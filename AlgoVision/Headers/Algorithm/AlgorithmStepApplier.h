#ifndef ASA_H
#define ASA_H

#include "AlgorithmStep.h"
#include "Graph.h"

class AlgorithmStepApplier {
public:
    explicit AlgorithmStepApplier(const std::shared_ptr<Graph>&);

    void apply(const AlgorithmStep&);

private:
    std::shared_ptr<Graph> m_graph;
};

#endif // ASA_H
