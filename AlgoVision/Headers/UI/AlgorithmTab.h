#ifndef ALGORITHMTAB_H
#define ALGORITHMTAB_H

#include <QWidget>

class QComboBox;
class QLineEdit;
class QPushButton;
class QToolButton;

class AlgorithmTab : public QWidget {
    Q_OBJECT

public:
    explicit AlgorithmTab(QWidget* parent = nullptr);
    ~AlgorithmTab() override = default;

private:
    void initLayout();
    void initIcons();
    void updateUiForAlgorithm(const QString& algorithmName);

    QComboBox*   m_algorithmCombo;

    QLineEdit*   m_startNodeEdit;
    QLineEdit*   m_endNodeEdit;

    QPushButton* m_helpBtn;

    QToolButton* m_prevBtn;
    QToolButton* m_playBtn;
    QToolButton* m_pauseBtn;
    QToolButton* m_nextBtn;
    QToolButton* m_restartBtn;
};

#endif // ALGORITHMTAB_H
