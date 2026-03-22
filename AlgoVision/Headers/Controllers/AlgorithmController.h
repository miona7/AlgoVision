#ifndef ALGORITHMCONTROLLER_H
#define ALGORITHMCONTROLLER_H

#include <QObject>

#include <vector>

#include "AlgorithmStep.h"
#include "AlgorithmError.h"
#include "AlgorithmStepApplier.h"

/**
 * @class AlgorithmController
 * @brief Controller that connects algorithm execution with the UI.
 *
 * This class acts as a "bridge" between the algorithm and UI controls
 * such as play, pause, next, previous, and reset.
 * It receives algorithm steps and manages their execution.
 */

class AlgorithmController : public QObject {
    Q_OBJECT
public:
    explicit AlgorithmController(AlgorithmStepApplier&, QObject* = nullptr);
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

    void onAlgorithmError(const AlgorithmError&);

signals:
    void requestErrorDialog(const AlgorithmError&);

private:
    AlgorithmStepApplier&      m_applier;
    std::vector<AlgorithmStep> m_steps;
    int                        m_currentIndex {-1};

    std::vector<AlgorithmStep> m_undoStack;
    std::vector<AlgorithmStep> m_redoStack;

    QString m_resultString;

    void undo();
    void redo();
};

#endif // ALGORITHMCONTROLLER_H
