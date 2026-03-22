#ifndef PRIM_H
#define PRIM_H

#include <map>
#include <queue>
#include <limits>
#include <optional>

#include "BFS.h"
#include "Algorithm.h"

class Prim : public Algorithm {
public:
    explicit Prim(const std::shared_ptr<Graph>);

    std::optional<AlgorithmError> checkConditions() const;
    std::optional<AlgorithmError> execute(unsigned = 0, unsigned = 0) override;

    QString resultString() const override;

private:
    int m_totalWeight{0};

    void prim();
};

#endif // PRIM_H
