#ifndef TARJAN_H
#define TARJAN_H

#include <map>
#include <stack>
#include <vector>
#include <memory>
#include <optional>

#include "Algorithm.h"

class Tarjan : public Algorithm {
public:
    explicit Tarjan(const std::shared_ptr<Graph>);

    std::optional<AlgorithmError> checkConditions() const;
    std::optional<AlgorithmError> execute(const AlgorithmParams&) override;

    QString getResultString() const override;

private:
    int                      m_arrivalTime{0};
    std::map<unsigned, int>  m_incomingNumbering;
    std::map<unsigned, int>  m_lowLink;
    std::stack<unsigned>     m_tourOrder; // traversal order
    std::map<unsigned, bool> m_onStack;
    std::map<unsigned, int>  m_components;       // node id -> component
    int                      m_numComponents{0}; // num of SCC

    void tarjan(unsigned);
    void init();
};

#endif // TARJAN_H
