#ifndef DFS_H
#define DFS_H

#include <iostream>
#include <map>
#include <optional>

#include "Algorithm.h"

class DFS : public Algorithm {
public:
    explicit DFS(const std::shared_ptr<Graph>);

    std::optional<AlgorithmError> checkConditions(unsigned) const;
    std::optional<AlgorithmError> execute(unsigned, unsigned = 0) override;

    const std::map<unsigned, bool>& getVisited() const;
    QString                         resultString() const override;

private:
    std::map<unsigned, bool> m_visited;
    std::vector<int>         m_order;
    void                     dfs(unsigned);
};

#endif // DFS_H
