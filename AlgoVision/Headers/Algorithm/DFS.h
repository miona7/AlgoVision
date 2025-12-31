#ifndef DFS_H
#define DFS_H

#include <iostream>
#include <map>
#include <stdexcept>

#include "Algorithm.h"

class DFS : public Algorithm {
public:
    explicit DFS(const std::shared_ptr<Graph>&);

    void checkConditions(unsigned) const;
    void execute(unsigned, unsigned idEndNode = 0) override;

private:
    void dfs(unsigned, std::map<unsigned, bool>&);
};

#endif // DFS_H
