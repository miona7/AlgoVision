#ifndef ALGORITHMTAB_H
#define ALGORITHMTAB_H

#include <QAbstractItemView>
#include <QColor>
#include <QComboBox>
#include <QFrame>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QString>
#include <QStyle>
#include <QTimer>
#include <QToolButton>
#include <QVBoxLayout>
#include <QWidget>

#include <optional>

#include "AlgorithmWorker.h"
#include "GraphController.h"
#include "AlgorithmController.h"

class AlgorithmTab : public QWidget {
    Q_OBJECT
public:
    explicit AlgorithmTab(std::shared_ptr<GraphController>, QWidget* = nullptr);
    ~AlgorithmTab() override = default;

public slots:
    void showAlgorithmErrorDialog(const AlgorithmError&);

signals:
    void graphEditAllowedChanged(bool);

private:
    struct AlgorithmConfig {
        QString  m_algorithmName;
        unsigned m_startNode;
        unsigned m_endNode;

        bool operator==(const AlgorithmConfig&) const;
        bool operator!=(const AlgorithmConfig&) const;
    };

    AlgorithmConfig selectedConfig() const;

    enum class RunState { Idle, Playing, Paused, Finished };

    QComboBox* m_algorithmCombo{nullptr};

    QWidget*   m_startRow{nullptr};
    QLabel*    m_startLabel{nullptr};
    QLineEdit* m_startNodeEdit{nullptr};

    QWidget*   m_endRow{nullptr};
    QLabel*    m_endLabel{nullptr};
    QLineEdit* m_endNodeEdit{nullptr};

    QLabel* m_noInputLabel{nullptr};

    QScrollArea* m_legendScroll{nullptr};

    QVBoxLayout* m_legendLayout{nullptr};
    QWidget*     m_legendContainer{nullptr};

    QPushButton* m_helpBtn{nullptr};

    QToolButton* m_prevBtn{nullptr};
    QToolButton* m_playBtn{nullptr};
    QToolButton* m_pauseBtn{nullptr};
    QToolButton* m_nextBtn{nullptr};
    QToolButton* m_restartBtn{nullptr};

    std::shared_ptr<GraphController> m_graphController;
    AlgorithmStepApplier             m_applier;
    AlgorithmController              m_algorithmController;
    AlgorithmWorker*                 m_worker{nullptr};
    QTimer*                          m_timer{nullptr};
    RunState                         m_state{RunState::Idle};
    std::optional<AlgorithmConfig>   m_currentConfig;
    bool                             m_graphEditAllowed{true};

    void     initLayout();
    void     initIcons();
    void     updateUiForAlgorithm(const QString&);
    void     updateLegendForAlgorithm(const QString&);
    QWidget* makeLegendItem(const QColor&, const QString&);

    void startTimerForPlay();
    void updateControls();
};

#endif // ALGORITHMTAB_H
