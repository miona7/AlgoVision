#ifndef ALGORITHMTAB_H
#define ALGORITHMTAB_H

#include <QWidget>

class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QToolButton;

class AlgorithmTab : public QWidget {
    Q_OBJECT

public:
    explicit AlgorithmTab(QWidget* = nullptr);
    ~AlgorithmTab() override = default;

private:
    QComboBox* m_algorithmCombo;

    QWidget*  m_startRow;
    QLabel*   m_startLabel;
    QLineEdit* m_startNodeEdit;

    QWidget*  m_endRow;
    QLabel*   m_endLabel;
    QLineEdit* m_endNodeEdit;

    QLabel* m_noInputLabel;

    QPushButton* m_helpBtn;

    QToolButton* m_prevBtn;
    QToolButton* m_playBtn;
    QToolButton* m_pauseBtn;
    QToolButton* m_nextBtn;
    QToolButton* m_restartBtn;

    void initLayout();
    void initIcons();
    void updateUiForAlgorithm(const QString&);
};

#endif // ALGORITHMTAB_H
