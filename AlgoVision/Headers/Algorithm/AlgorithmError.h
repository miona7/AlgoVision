#ifndef ALGORITHMERROR_H
#define ALGORITHMERROR_H

#include <string>

/**
 * @enum AlgorithmErrorType
 * @brief Represents different types of errors that can occur during algorithm execution.
 *
 * These errors are used to indicate problems like missing nodes, invalid graph type,
 * or algorithm-specific failures such as negative cycles or unreachable paths.
 */

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

/**
 * @struct AlgorithmError
 * @brief Represents an error that occurred during algorithm execution.
 *
 * Contains the error type and an optional descriptive message.
 */

struct AlgorithmError {
    AlgorithmErrorType m_type;
    std::string        m_message;
};

#endif // ALGORITHMERROR_H
