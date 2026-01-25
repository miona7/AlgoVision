#include "AlgorithmController.h"

AlgorithmController::AlgorithmController(AlgorithmStepApplier& applier, QObject* parent)
    : m_applier(applier), QObject(parent) {
}

void AlgorithmController::loadSteps(const std::vector<AlgorithmStep>& steps) {
    m_steps        = steps;
    m_currentIndex = -1;
    m_undoStack.clear();
    m_redoStack.clear();
}

void AlgorithmController::nextStep() {
    if(m_currentIndex + 1 >= m_steps.size()) {
        return;
    }
    ++m_currentIndex;
    AlgorithmStep& step = m_steps[m_currentIndex];
    m_applier.apply(step);
    m_undoStack.push_back(step);
    m_redoStack.clear();
}

void AlgorithmController::prevStep() {
    if(m_undoStack.empty()) {
        return;
    }
    AlgorithmStep& step = m_undoStack.back();
    m_applier.undo(step);
    m_undoStack.pop_back();
    m_redoStack.push_back(step);
    m_currentIndex--;
}

void AlgorithmController::undo() {
    prevStep();
}

void AlgorithmController::redo() {
    if(m_redoStack.empty()) {
        return;
    }
    AlgorithmStep& step = m_redoStack.back();
    m_applier.apply(step);
    m_redoStack.pop_back();
    m_undoStack.push_back(step);
    m_currentIndex++;
}

void AlgorithmController::reset() {
    while(!m_undoStack.empty()) {
        AlgorithmStep& step = m_undoStack.back();
        m_applier.undo(step);
        m_undoStack.pop_back();
    }
    m_redoStack.clear();
    m_currentIndex = -1;
}

bool AlgorithmController::isFinished() const {
    return m_currentIndex + 1 >= m_steps.size();
}
