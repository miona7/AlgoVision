#ifndef FW_H
#define FW_H

#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <limits>

#include "Algorithm.h"
#include "BellmanFord.h"

class FloydWarshall : public Algorithm {
public:
    explicit FloydWarshall(const std::shared_ptr<Graph>&);

    bool checkConditions() const override;
    void execute(unsigned idStartNode = 0, unsigned idEndNode = 0) override;

private:
    std::map<unsigned, std::map<unsigned, int>> m_distances;

    void floydWarshall();
};

#endif // FW_H
