#ifndef BELLMANFORD_H
#define BELLMANFORD_H

#include <map>
#include <vector>
#include <limits>
#include <memory>
#include <optional>

#include "Algorithm.h"

class BellmanFord : public Algorithm {
public:
    explicit BellmanFord(const std::shared_ptr<Graph>);

    std::optional<AlgorithmError> checkConditions(unsigned) const;
    std::optional<AlgorithmError> execute(const AlgorithmParams&) override;

    QString getResultString() const override;
    bool hasNegativeCycle() const;

private:
    bool                    m_hasNegativeCycle{false};
    std::map<unsigned, int> m_minDistance;

    void bellmanFord(unsigned);
};

#endif // BELLMANFORD_H
