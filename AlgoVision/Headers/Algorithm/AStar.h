#ifndef ASTAR_H
#define ASTAR_H

#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <optional>
#include <queue>
#include <vector>

#include "Algorithm.h"
#include "AlgorithmError.h"

class AStar : public Algorithm {
public:
    explicit AStar(const std::shared_ptr<Graph>);

    std::optional<AlgorithmError> checkConditions(unsigned, unsigned) const;
    std::optional<AlgorithmError> execute(unsigned, unsigned) override;

    QString resultString() const override;

    const std::vector<unsigned>& getPath() const;
    int                          getTotalCost() const;

private:
    std::optional<AlgorithmError> aStar(unsigned, unsigned);
    int                           heuristic(unsigned, unsigned) const;

    std::vector<unsigned> m_path;
    int                   m_totalCost = 0;
};

#endif // ASTAR_H
