#ifndef BFS_H
#define BFS_H

#include <map>
#include <queue>
#include <vector>
#include <optional>

#include "Algorithm.h"

class BFS : public Algorithm {
public:
    explicit BFS(const std::shared_ptr<Graph>);

    std::optional<AlgorithmError> checkConditions(unsigned) const;
    std::optional<AlgorithmError> execute(const AlgorithmParams&) override;

    QString getResultString() const override;
    const std::map<unsigned, bool>& getVisited() const;

private:
    std::map<unsigned, bool> m_visited;
    std::vector<unsigned>    m_order;

    void bfs(unsigned);
};

#endif // BFS_H
