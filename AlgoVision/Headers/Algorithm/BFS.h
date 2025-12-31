#ifndef BFS_H
#define BFS_H

#include <iostream>
#include <queue>
#include <stdexcept>

#include "Algorithm.h"

class BFS : public Algorithm {
public:
    explicit BFS(const std::shared_ptr<Graph>&);

    bool checkConditions() const override;
    void execute(unsigned idStartNode = 0, unsigned idEndNode = 0) override;

private:
    void bfs(unsigned);
};

#endif // BFS_H
