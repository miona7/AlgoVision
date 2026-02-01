#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <iostream>
#include <limits>
#include <map>
#include <queue>

#include "Algorithm.h"

class Dijkstra : public Algorithm {
public:
    explicit Dijkstra(const std::shared_ptr<Graph>);

    std::optional<AlgorithmError> checkConditions(unsigned) const;
    std::optional<AlgorithmError> execute(unsigned, unsigned = 0) override;

private:
    void dijkstra(unsigned);
};

#endif // DIJKSTRA_H
