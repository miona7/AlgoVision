#ifndef KAHN_H
#define KAHN_H

#include <map>
#include <queue>
#include <vector>
#include <memory>
#include <optional>

#include "Algorithm.h"

class Kahn : public Algorithm {
public:
    explicit Kahn(const std::shared_ptr<Graph>);

    std::optional<AlgorithmError> checkConditions() const;
    std::optional<AlgorithmError> execute(const AlgorithmParams&) override;

    QString getResultString() const override;
    const std::vector<unsigned>& getSorted() const;

private:
    std::vector<unsigned> m_sorted;

    std::optional<AlgorithmError> kahn();
};

#endif // KAHN_H
