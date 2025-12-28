#include "Algorithm.h"

#include <stack>
#include <stdexcept>
#include <vector>

class Tarjan : public Algorithm {
public:
    bool checkConditions() const override;
    void execute(Node* startNode = nullptr, Node* endNode = nullptr) override;

private:
    int               m_arrivalTime = 1;
    std::vector<int>  m_incomingNumbering;
    std::vector<int>  m_lowLink;
    std::stack<int>   m_tourOrder;
    std::vector<bool> m_onStack;
    std::vector<int>  m_components;

    void tarjan(int, Node*, int&);
};
