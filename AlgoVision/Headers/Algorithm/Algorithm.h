#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <memory>
#include <vector>
#include <optional>

#include "AlgorithmStep.h"
#include "Graph.h"
#include "AlgorithmError.h"

class Algorithm {
public:
    explicit Algorithm(const std::shared_ptr<Graph>);
    virtual ~Algorithm() = default;

    virtual std::optional<AlgorithmError> execute(unsigned = 0, unsigned = 0) = 0;

    const std::vector<AlgorithmStep>& getSteps() const;

protected:
    std::shared_ptr<Graph>     m_graph;
    std::vector<AlgorithmStep> m_steps;

    void addStep(const AlgorithmStep&);
    void clearSteps();
};

#endif // ALGORITHM_H
