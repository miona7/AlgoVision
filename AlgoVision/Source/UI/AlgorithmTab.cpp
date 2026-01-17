#include "AlgorithmTab.h"

#include <QComboBox>
#include <QFrame>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QStyle>
#include <QToolButton>
#include <QVBoxLayout>

AlgorithmTab::AlgorithmTab(QWidget* parent)
    : QWidget(parent), m_algorithmCombo(new QComboBox(this)),
      m_startRow(new QWidget(this)), m_startLabel(new QLabel("start node:", this)),
      m_startNodeEdit(new QLineEdit(this)), m_endRow(new QWidget(this)),
      m_endLabel(new QLabel("end node:", this)), m_endNodeEdit(new QLineEdit(this)),
      m_noInputLabel(new QLabel(this)), m_helpBtn(new QPushButton("help", this)),
      m_prevBtn(new QToolButton(this)), m_playBtn(new QToolButton(this)),
      m_pauseBtn(new QToolButton(this)), m_nextBtn(new QToolButton(this)),
      m_restartBtn(new QToolButton(this)) {
    initLayout();
    initIcons();

    updateUiForAlgorithm(m_algorithmCombo->currentText());

    connect(m_algorithmCombo, &QComboBox::currentTextChanged, this,
            &AlgorithmTab::updateUiForAlgorithm);

    connect(m_helpBtn, &QPushButton::clicked, this, [this]() {
        const QString helpText =
            "Algorithm Tab — Help\n\n"
            "Use this tab to select an algorithm and control its execution on the graph displayed "
            "in the main area.\n\n"
            "1. Choose algorithm\n"
            "Select an algorithm from the drop-down list.\n\n"
            "2. Algorithm attributes\n"
            "If the selected algorithm requires extra input, enter the requested values (e.g., "
            "start node / end node).\n"
            "If no parameters are needed, you will see: \"No additional input needed.\".\n\n"
            "3. Help\n"
            "Press Help at any time to review these instructions.\n\n"
            "4. Run algorithm\n"
            "Use the control buttons to navigate through the algorithm:\n"
            "- Previous step: go one step back\n"
            "- Play: run continuously\n"
            "- Pause: pause execution\n"
            "- Next step: advance one step\n"
            "- Restart: reset execution to the beginning\n\n";

        QMessageBox::information(this, "Algorithm Tab Help", helpText);
    });
}

void AlgorithmTab::initLayout() {
    auto* mainLayout = new QVBoxLayout(this);

    // algorithm choice
    auto* chooseAlgoBox    = new QGroupBox("choose algorithm", this);
    auto* chooseAlgoLayout = new QVBoxLayout(chooseAlgoBox);

    m_algorithmCombo->addItem("A*");
    m_algorithmCombo->addItem("Bellman-Ford");
    m_algorithmCombo->addItem("BFS");
    m_algorithmCombo->addItem("DFS");
    m_algorithmCombo->addItem("Dijkstra");
    m_algorithmCombo->addItem("Floyd-Warshall");
    m_algorithmCombo->addItem("Kahn");
    m_algorithmCombo->addItem("Prim");
    m_algorithmCombo->addItem("Tarjan");

    chooseAlgoLayout->addWidget(m_algorithmCombo);
    mainLayout->addWidget(chooseAlgoBox);

    // algorithm attributes
    auto* attributesBox    = new QGroupBox("algorithm attributes", this);
    auto* attributesLayout = new QVBoxLayout(attributesBox);

    // start row (label + edit) as one widget
    auto* startRowLayout = new QHBoxLayout(m_startRow);
    startRowLayout->addWidget(m_startLabel);
    startRowLayout->addWidget(m_startNodeEdit);
    m_startNodeEdit->setPlaceholderText("e.g. 0");
    attributesLayout->addWidget(m_startRow);

    // end row (label + edit) as one widget
    auto* endRowLayout = new QHBoxLayout(m_endRow);
    endRowLayout->addWidget(m_endLabel);
    endRowLayout->addWidget(m_endNodeEdit);
    m_endNodeEdit->setPlaceholderText("e.g. 5");
    attributesLayout->addWidget(m_endRow);

    // message when no input needed
    m_noInputLabel->setText("No additional input needed.");
    m_noInputLabel->setWordWrap(true);
    m_noInputLabel->hide();
    attributesLayout->addWidget(m_noInputLabel);

    mainLayout->addWidget(attributesBox);

    // help
    m_helpBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    mainLayout->addWidget(m_helpBtn);

    // separator
    auto* separator = new QFrame(this);
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    mainLayout->addWidget(separator);

    // run algorithm
    auto* runLabel = new QLabel("run algorithm", this);
    mainLayout->addWidget(runLabel);

    auto* runLayout = new QHBoxLayout();
    runLayout->setSpacing(6);

    m_prevBtn->setToolTip("previous step");
    m_playBtn->setToolTip("play");
    m_pauseBtn->setToolTip("pause");
    m_nextBtn->setToolTip("next step");
    m_restartBtn->setToolTip("restart");

    m_prevBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_playBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_pauseBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_nextBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_restartBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    runLayout->addWidget(m_prevBtn);
    runLayout->addWidget(m_playBtn);
    runLayout->addWidget(m_pauseBtn);
    runLayout->addWidget(m_nextBtn);
    runLayout->addWidget(m_restartBtn);

    mainLayout->addLayout(runLayout);
    mainLayout->addStretch();
}

void AlgorithmTab::initIcons() {
    m_prevBtn->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    m_playBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    m_pauseBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    m_nextBtn->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    m_restartBtn->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
}

void AlgorithmTab::updateUiForAlgorithm(const QString& algorithmName) {
    const bool needsStart = algorithmName == "BFS" || algorithmName == "DFS" ||
                            algorithmName == "Dijkstra" || algorithmName == "Bellman-Ford" ||
                            algorithmName == "Prim" || algorithmName == "A*";

    const bool needsEnd = algorithmName == "A*";

    // show/hide whole rows
    if(!needsStart && !needsEnd) {
        m_startNodeEdit->clear();
        m_endNodeEdit->clear();

        m_startRow->hide();
        m_endRow->hide();
        m_noInputLabel->show();
        return;
    }

    m_noInputLabel->hide();

    if(needsStart) {
        m_startRow->show();
        m_startNodeEdit->setPlaceholderText("e.g. 0");
    } else {
        m_startNodeEdit->clear();
        m_startRow->hide();
    }

    if(needsEnd) {
        m_endRow->show();
        m_endNodeEdit->setPlaceholderText("e.g. 5");
    } else {
        m_endNodeEdit->clear();
        m_endRow->hide();
    }
}
