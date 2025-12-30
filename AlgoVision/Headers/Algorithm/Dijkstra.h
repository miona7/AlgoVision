#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <iostream>
#include <map>
#include <queue>
#include <limits>

#include "Algorithm.h"

class Dijkstra : public Algorithm {
public:
    explicit Dijkstra(const std::shared_ptr<Graph>&);

    bool checkConditions() const override;
    void execute(unsigned idStartNode = 0, unsigned idEndNode = 0) override;

private:
    void dijkstra(unsigned);
};

#endif // DIJKSTRA_H
