#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <vector>
#include <memory>
#include <optional>

#include "Graph.h"
#include "AlgorithmStep.h"
#include "AlgorithmError.h"

class Algorithm {
public:
    struct Params {
        std::optional<unsigned> m_startNode;
        std::optional<unsigned> m_endNode;

        Params() = default;
        Params(unsigned start) : m_startNode(start) {
        }
        Params(unsigned start, unsigned end) : m_startNode(start), m_endNode(end) {
        }
    };
    using AlgorithmParams = Algorithm::Params;

    explicit Algorithm(const std::shared_ptr<Graph>);
    virtual ~Algorithm() = default;

    virtual std::optional<AlgorithmError> execute(const AlgorithmParams&) = 0;

    virtual QString                   getResultString() const = 0;
    const std::vector<AlgorithmStep>& getSteps() const;

protected:
    std::shared_ptr<Graph>     m_graph;
    std::vector<AlgorithmStep> m_steps;

    void addStep(const AlgorithmStep&);
    void clearSteps();
};

#endif // ALGORITHM_H
