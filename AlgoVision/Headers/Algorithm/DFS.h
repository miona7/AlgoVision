#ifndef DFS_H
#define DFS_H

#include <stdexcept>
#include <map>

#include "Algorithm.h"

class DFS : public Algorithm {
public:
    DFS(std::shared_ptr<Graph>);

    bool checkConditions() const override;

    // razmisliti da bude private
    void execute(const unsigned idStartNode = 0, const unsigned idEndNode = 0) override;

private:
    void dfs(unsigned, std::map<unsigned, bool>&);
};

#endif // DFS_H
