#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <map>
#include <queue>
#include <numeric>

#include "Algorithm.h"

class Dijkstra : public Algorithm {
public:
    explicit Dijkstra(std::shared_ptr<Graph> g);

    bool checkConditions() const override;
    void execute(const unsigned idStartNode = 0, const unsigned idEndNode = 0) override;

private:
    void dijkstra(unsigned);
};

#endif // DIJKSTRA_H
