#ifndef TARJAN_H
#define TARJAN_H

#include <iostream>
#include <vector>
#include <map>
#include <stack>
#include <memory>
#include <stdexcept>

#include "Algorithm.h"

class Tarjan : public Algorithm {
public:
    explicit Tarjan(const std::shared_ptr<Graph>&);

    bool checkConditions() const override;
    void execute(unsigned idStartNode = 0, unsigned idEndNode = 0) override;

private:
    int                      m_arrivalTime;       // vreme dolaska
    std::map<unsigned, int>  m_incomingNumbering; // dolazna numeracija
    std::map<unsigned, int>  m_lowLink;           // lowlink vrednost
    std::stack<unsigned>     m_tourOrder;         // redosled u obilasku
    std::map<unsigned, bool> m_onStack;
    std::map<unsigned, int>  m_components;        // id cvora -> komponenta kojoj pripada

    void tarjan(unsigned, int&);
    void init();
};

#endif // TARJAN_H
