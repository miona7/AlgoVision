#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <iostream>
#include <limits>
#include <map>
#include <optional>
#include <queue>

#include "Algorithm.h"

class Dijkstra : public Algorithm {
public:
    explicit Dijkstra(const std::shared_ptr<Graph>);

    std::optional<AlgorithmError> checkConditions(unsigned) const;
    std::optional<AlgorithmError> execute(unsigned, unsigned = 0) override;

    QString resultString() const override;

private:
    void    dijkstra(unsigned);
    QString m_resultString;
};

#endif // DIJKSTRA_H
