#ifndef ASTAR_H
#define ASTAR_H

#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <queue>
#include <vector>

#include "Algorithm.h"

class AStar : public Algorithm {
public:
    explicit AStar(const std::shared_ptr<Graph>);

    void checkConditions(unsigned, unsigned) const;
    void execute(unsigned, unsigned) override;
    QString resultString() const;


    const std::vector<unsigned>& getPath() const;
    int                          getTotalCost() const;

private:
    void aStar(unsigned, unsigned);
    int  heuristic(unsigned, unsigned) const;

    std::vector<unsigned> m_path;
    int                   m_totalCost = 0;
};

#endif // ASTAR_H
