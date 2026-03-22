#ifndef ASTAR_H
#define ASTAR_H

#include <map>
#include <queue>
#include <vector>
#include <cmath>
#include <limits>
#include <memory>
#include <optional>

#include "Algorithm.h"
#include "AlgorithmError.h"

class AStar : public Algorithm {
public:
    explicit AStar(const std::shared_ptr<Graph>);

    std::optional<AlgorithmError> checkConditions(unsigned, unsigned) const;
    std::optional<AlgorithmError> execute(unsigned, unsigned) override;

    const std::vector<unsigned>& getPath() const;
    int                          getTotalCost() const;
    QString                      resultString() const override;

private:
    std::vector<unsigned> m_path;
    int                   m_totalCost = 0;

    std::optional<AlgorithmError> aStar(unsigned, unsigned);
    int                           heuristic(unsigned, unsigned) const;
};

#endif // ASTAR_H
