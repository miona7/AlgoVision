#ifndef ALGORITHM_CONTROLLER_H
#define ALGORITHM_CONTROLLER_H

#include <QObject>

#include <vector>

#include "AlgorithmError.h"
#include "AlgorithmStep.h"
#include "AlgorithmStepApplier.h"

class AlgorithmController : public QObject {
    Q_OBJECT
public:
    explicit AlgorithmController(AlgorithmStepApplier&, QObject* = nullptr);
    ~AlgorithmController() override = default;

    void clear();

public slots:
    void loadSteps(const std::vector<AlgorithmStep>&);

    void nextStep();
    void prevStep();
    void reset();
    bool isFinished() const;

    void onAlgorithmError(const AlgorithmError& error);

signals:
    void requestErrorDialog(const AlgorithmError& error, bool allowContinue);

private:
    // kontroler ne poseduje applier, samo ga koristi
    // applier ima stanja, tj Graph, i zato mi necemo da konstruisemo applier, nego samo da ga
    // koristimo
    AlgorithmStepApplier&      m_applier;
    std::vector<AlgorithmStep> m_steps;
    int                        m_currentIndex = -1; // na pocetku nemamo stanja

    std::vector<AlgorithmStep> m_undoStack;
    std::vector<AlgorithmStep> m_redoStack;

    void undo();
    void redo();
};

#endif // ALGORITHM_CONTROLLER_H
