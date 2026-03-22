#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <map>
#include <queue>
#include <limits>
#include <optional>

#include "Algorithm.h"

class Dijkstra : public Algorithm {
public:
    explicit Dijkstra(const std::shared_ptr<Graph>);

    std::optional<AlgorithmError> checkConditions(unsigned) const;
    std::optional<AlgorithmError> execute(unsigned, unsigned = 0) override;

    QString resultString() const override;

private:
    QString m_resultString;

    void dijkstra(unsigned);
};

#endif // DIJKSTRA_H
