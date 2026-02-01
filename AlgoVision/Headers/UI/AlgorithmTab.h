#ifndef ALGORITHMTAB_H
#define ALGORITHMTAB_H

#include <QString>
#include <QTimer>
#include <QWidget>

#include <optional>

#include "AlgorithmController.h"
#include "AlgorithmWorker.h"

class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QToolButton;

class AlgorithmTab : public QWidget {
    Q_OBJECT

public:
    explicit AlgorithmTab(std::shared_ptr<Graph>, QWidget* = nullptr);
    ~AlgorithmTab() override = default;

public slots:
    void showAlgorithmErrorDialog(const AlgorithmError& error, bool allowContinue);

signals:
    void errorDialogContinue();
    void errorDialogCancelled();

private:
    struct AlgorithmConfig {
        QString m_algorithmName;
        int     m_startNode;
        int     m_endNode;

        bool operator==(const AlgorithmConfig&) const;
        bool operator!=(const AlgorithmConfig&) const;
    };

    AlgorithmConfig selectedConfig() const;

    enum class RunState { Idle, Playing, Paused, Finished };

    QComboBox* m_algorithmCombo;

    QWidget*   m_startRow;
    QLabel*    m_startLabel;
    QLineEdit* m_startNodeEdit;

    QWidget*   m_endRow;
    QLabel*    m_endLabel;
    QLineEdit* m_endNodeEdit;

    QLabel* m_noInputLabel;

    QPushButton* m_helpBtn;

    QToolButton* m_prevBtn;
    QToolButton* m_playBtn;
    QToolButton* m_pauseBtn;
    QToolButton* m_nextBtn;
    QToolButton* m_restartBtn;

    std::shared_ptr<Graph>         m_graph;
    AlgorithmStepApplier           m_applier;
    AlgorithmController            m_algorithmController;
    AlgorithmWorker*               m_worker = nullptr;
    QTimer*                        m_timer  = nullptr;
    RunState                       m_state  = RunState::Idle;
    std::optional<AlgorithmConfig> m_currentConfig;

    void initLayout();
    void initIcons();
    void updateUiForAlgorithm(const QString&);

    void startTimerForPlay();
};

#endif // ALGORITHMTAB_H
