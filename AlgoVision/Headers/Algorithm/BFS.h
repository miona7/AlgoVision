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

    void    checkConditions(unsigned) const;
    void    execute(unsigned, unsigned = 0) override;
    QString resultString() const override;

    const std::map<unsigned, bool>& getVisited() const;

private:
    std::map<unsigned, bool> m_visited;
    std::vector<int>         m_order;
    void                     bfs(unsigned);
};

#endif // BFS_H
