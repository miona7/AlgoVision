#ifndef BFS_H
#define BFS_H

#include <iostream>
#include <map>
#include <queue>
#include <stdexcept>

#include "Algorithm.h"

class BFS : public Algorithm {
public:
    explicit BFS(const std::shared_ptr<Graph>);

    std::optional<AlgorithmError> checkConditions(unsigned) const;
    std::optional<AlgorithmError> execute(unsigned, unsigned = 0) override;

    const std::map<unsigned, bool>& getVisited() const;

private:
    std::map<unsigned, bool> m_visited;

    void bfs(unsigned);
};

#endif // BFS_H
