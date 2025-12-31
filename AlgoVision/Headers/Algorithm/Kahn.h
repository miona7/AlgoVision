#ifndef KAHN_H
#define KAHN_H

#include <iostream>
#include <memory>
#include <map>
#include <queue>
#include <vector>

#include "Algorithm.h"

class Kahn : public Algorithm {
public:
    explicit Kahn(const std::shared_ptr<Graph>&);

    void checkConditions() const;
    void execute(unsigned = 0, unsigned = 0) override;

    const std::vector<unsigned>& getSorted() const;

private:
    std::vector<unsigned> m_sorted;

    void kahn();
};

#endif // KAHN_H
