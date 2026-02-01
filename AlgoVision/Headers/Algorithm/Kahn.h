#ifndef KAHN_H
#define KAHN_H

#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <vector>
#include <optional>

#include "Algorithm.h"

class Kahn : public Algorithm {
public:
    explicit Kahn(const std::shared_ptr<Graph>);

    std::optional<AlgorithmError> checkConditions() const;
    std::optional<AlgorithmError> execute(unsigned = 0, unsigned = 0) override;

    const std::vector<unsigned>& getSorted() const;

private:
    std::vector<unsigned> m_sorted;

    std::optional<AlgorithmError> kahn();
};

#endif // KAHN_H
