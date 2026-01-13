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
    void execute(unsigned, unsigned = 0) override;

    const std::map<unsigned, bool>& getVisited() const;

private:
    std::map<unsigned, bool> m_visited;

    void dfs(unsigned, std::optional<unsigned>);
};

#endif // DFS_H
