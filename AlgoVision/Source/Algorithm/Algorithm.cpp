#include "Algorithm.h"

Algorithm::Algorithm(const std::shared_ptr<Graph>& g) : m_graph(g) {
}

const std::vector<AlgorithmStep>& Algorithm::getSteps() const {
    return m_steps;
}

void Algorithm::addStep(const AlgorithmStep& step) {
    m_steps.push_back(step);
}

void Algorithm::clearSteps()
{
    m_steps.clear();
}
