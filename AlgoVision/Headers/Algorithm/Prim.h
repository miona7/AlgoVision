#ifndef PRIM_H
#define PRIM_H

#include <iostream>
#include <limits>
#include <map>
#include <optional>
#include <queue>

#include "Algorithm.h"
#include "BFS.h"

class Prim : public Algorithm {
public:
    explicit Prim(const std::shared_ptr<Graph>);

    void checkConditions() const;
    void execute(unsigned = 0, unsigned = 0) override;
    QString resultString() const;
    std::vector<std::pair<unsigned,unsigned>> m_mstEdges;

    double m_totalWeight = 0;


private:
    void prim();
};

#endif // PRIM_H
