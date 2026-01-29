#ifndef FW_H
#define FW_H

#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <stdexcept>

#include "Algorithm.h"
#include "BellmanFord.h"

class FloydWarshall : public Algorithm {
public:
    explicit FloydWarshall(const std::shared_ptr<Graph>);

    void checkConditions() const;
    void execute(unsigned, unsigned) override;

private:
    std::map<unsigned, std::map<unsigned, int>> m_distances;

    void floydWarshall();
};

#endif // FW_H
