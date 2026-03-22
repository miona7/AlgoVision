#ifndef FLOYDWARSHALL_H
#define FLOYDWARSHALL_H

#include <map>
#include <queue>
#include <limits>
#include <optional>

#include "Algorithm.h"

class FloydWarshall : public Algorithm {
public:
    explicit FloydWarshall(const std::shared_ptr<Graph>);

    std::optional<AlgorithmError> checkConditions() const;
    std::optional<AlgorithmError> execute(unsigned = 0, unsigned = 0) override;

    QString resultString() const override;

private:
    std::map<unsigned, std::map<unsigned, int>> m_distances;

    std::optional<AlgorithmError> floydWarshall();
};

#endif // FLOYDWARSHALL_H
