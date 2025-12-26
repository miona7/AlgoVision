#ifndef DFS_H
#define DFS_H

#include "Algorithm.h"

class DFS : public Algorithm {
public:
    bool checkConditions() const override;
    void execute(Node* startNode = nullptr, Node* endNode = nullptr) override;

private:
    void dfs(int nodeId, Node* node, std::vector<bool>& visited);
};

#endif // DFS_H
