#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <memory>
#include <vector>

#include "AlgorithmStep.h"
#include "Graph.h"

class Algorithm {
public:
    explicit Algorithm(const std::shared_ptr<Graph>);
    virtual ~Algorithm() = default;

    virtual void execute(unsigned = 0, unsigned = 0) = 0;

    const std::vector<AlgorithmStep>& getSteps() const;

    virtual QString resultString() const = 0;


protected:
    std::shared_ptr<Graph>     m_graph;
    std::vector<AlgorithmStep> m_steps;
    QString m_resultString;


    void addStep(const AlgorithmStep&);
    void clearSteps();
};

#endif // ALGORITHM_H
