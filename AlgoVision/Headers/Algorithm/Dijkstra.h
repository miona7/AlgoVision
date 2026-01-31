#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <iostream>
#include <limits>
#include <map>
#include <queue>

#include "Algorithm.h"

class Dijkstra : public Algorithm {
public:
    explicit Dijkstra(const std::shared_ptr<Graph>);

    void checkConditions(unsigned) const;
    void execute(unsigned, unsigned = 0) override;
    QString resultString() const;
    std::map<unsigned, double> m_dist;



private:
    void dijkstra(unsigned);
    QString m_resultString;

};


#endif // DIJKSTRA_H
