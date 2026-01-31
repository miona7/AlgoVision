#ifndef TARJAN_H
#define TARJAN_H

#include <iostream>
#include <map>
#include <memory>
#include <stack>
#include <stdexcept>
#include <vector>

#include "Algorithm.h"

class Tarjan : public Algorithm {
public:
    explicit Tarjan(const std::shared_ptr<Graph>);

    void    checkConditions() const;
    void    execute(unsigned = 0, unsigned = 0) override;
    QString resultString() const override;

    int m_sccCount = 0;

private:
    int                      m_arrivalTime = 0;   // vreme dolaska
    std::map<unsigned, int>  m_incomingNumbering; // dolazna numeracija
    std::map<unsigned, int>  m_lowLink;           // lowlink vrednost
    std::stack<unsigned>     m_tourOrder;         // redosled u obilasku
    std::map<unsigned, bool> m_onStack;
    std::map<unsigned, int>  m_components; // id cvora -> komponenta kojoj pripada

    void tarjan(unsigned, int&);
    void init();
};

#endif // TARJAN_H
