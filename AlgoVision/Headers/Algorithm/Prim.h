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
    void execute(unsigned, unsigned) override;

private:
    void prim();
};

#endif // PRIM_H
