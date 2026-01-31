#ifndef ALGORITHM_CONTROLLER_H
#define ALGORITHM_CONTROLLER_H

#include <QObject>
#include <vector>

#include "AlgorithmStep.h"
#include "AlgorithmStepApplier.h"

class AlgorithmController : public QObject {
    Q_OBJECT
public:
    AlgorithmController(AlgorithmStepApplier&, QObject* = nullptr);
    ~AlgorithmController() override = default;

    void    clear();
    QString resultString() const;

public slots:
    void loadSteps(const std::vector<AlgorithmStep>&);

    void nextStep();
    void prevStep();
    void reset();
    bool isFinished() const;
    void setResultString(const QString&);

private:
    // kontroler ne poseduje applier, samo ga koristi
    AlgorithmStepApplier&      m_applier;
    std::vector<AlgorithmStep> m_steps;
    int                        m_currentIndex = -1; // na pocetku nemamo stanja

    std::vector<AlgorithmStep> m_undoStack;
    std::vector<AlgorithmStep> m_redoStack;

    QString m_resultString;

    void undo();
    void redo();
};

#endif // ALGORITHM_CONTROLLER_H
