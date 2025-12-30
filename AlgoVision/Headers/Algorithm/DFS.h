#ifndef DFS_H
#define DFS_H

#include <map>
#include <stdexcept>

#include "Algorithm.h"

class DFS : public Algorithm {
public:
    explicit DFS(const std::shared_ptr<Graph>&);

    bool checkConditions() const override;

    // razmisliti da bude private
    void execute(unsigned idStartNode = 0, unsigned idEndNode = 0) override;

private:
    void dfs(unsigned, std::map<unsigned, bool>&);
};

#endif // DFS_H
