#ifndef BF_H
#define BF_H

#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <optional>
#include <stdexcept>
#include <vector>

#include "Algorithm.h"

class BellmanFord : public Algorithm {
public:
    explicit BellmanFord(const std::shared_ptr<Graph>);

    std::optional<AlgorithmError> checkConditions(unsigned) const;
    std::optional<AlgorithmError> execute(unsigned, unsigned = 0) override;

    bool hasNegativeCycle() const;

private:
    bool                    m_hasNegativeCycle = false;
    std::map<unsigned, int> m_minDistance;

    void bellmanFord(unsigned);
};

#endif // BF_H
