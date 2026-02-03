#ifndef FW_H
#define FW_H

#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <stdexcept>

#include "Algorithm.h"
#include "BellmanFord.h"

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

#endif // FW_H
