#ifndef BF_H
#define BF_H

#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <stdexcept>
#include <limits>

#include "Algorithm.h"

class BellmanFord : public Algorithm {
public:
    explicit BellmanFord(const std::shared_ptr<Graph>&);

    bool checkConditions() const override;
    void execute(unsigned idStartNode = 0, unsigned idEndNode = 0) override;

    bool hasNegativeCycle() const;

private:
    bool m_hasNegativeCycle;
    std::map<unsigned, int> m_minDistance;

    void bellmanFord(unsigned);
};

#endif // BF_H
