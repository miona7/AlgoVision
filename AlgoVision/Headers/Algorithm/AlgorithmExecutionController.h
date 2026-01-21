#ifndef EXECUTION_H
#define EXECUTION_H

#include <vector>

#include <QObject>

#include "AlgorithmStep.h"
#include "AlgorithmStepApplier.h"

class AlgorithmExecutionController : public QObject {
    Q_OBJECT
public:
    AlgorithmExecutionController(AlgorithmStepApplier&, QObject* = nullptr);
    ~AlgorithmExecutionController() override = default;

public slots:
    void loadSteps(const std::vector<AlgorithmStep>&);

    void nextStep();
    void prevStep();
    void reset();
    bool isFinished() const;

private:
    // kontroler ne poseduje applier, samo ga koristi
    // applier ima stanja, tj Graph, i zato mi necemo da konstruisemo applier, nego samo da ga koristimo
    AlgorithmStepApplier& m_applier;
    std::vector<AlgorithmStep> m_steps;
    int m_currentIndex = -1; // na pocetku nemamo stanja

    std::vector<AlgorithmStep> m_undoStack;
    std::vector<AlgorithmStep> m_redoStack;

    void undo();
    void redo();
};

#endif // EXECUTION_H

