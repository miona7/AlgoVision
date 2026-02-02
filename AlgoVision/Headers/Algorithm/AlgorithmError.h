#ifndef ALGORITHMERROR_H
#define ALGORITHMERROR_H

#include <string>

enum class AlgorithmErrorType {
    GraphNotInitialized,
    GraphTypeInvalid,
    StartNodeMissing,
    GoalNodeMissing,
    NegativeEdgeWeights,
    NoPathFound,
    GraphHasNegativeCycle,
    GraphHasCycle,
    GraphNotConnected
};

struct AlgorithmError {
    AlgorithmErrorType m_type;
    std::string        m_message;
};

#endif // ALGORITHMERROR_H
