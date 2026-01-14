#include "AlgorithmTab.h"

#include <QComboBox>
#include <QFormLayout>
#include <QFrame>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStyle>
#include <QToolButton>
#include <QVBoxLayout>

AlgorithmTab::AlgorithmTab(QWidget* parent)
    : QWidget(parent), m_algorithmCombo(new QComboBox(this)), m_startNodeEdit(new QLineEdit(this)),
      m_endNodeEdit(new QLineEdit(this)), m_helpBtn(new QPushButton("help", this)),
      m_prevBtn(new QToolButton(this)), m_playBtn(new QToolButton(this)),
      m_pauseBtn(new QToolButton(this)), m_nextBtn(new QToolButton(this)),
      m_restartBtn(new QToolButton(this)) {
    initLayout();
    initIcons();

    updateUiForAlgorithm(m_algorithmCombo->currentText());

    connect(m_algorithmCombo, &QComboBox::currentTextChanged, this,
            &AlgorithmTab::updateUiForAlgorithm);
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
    auto* attributesBox = new QGroupBox("algorithm attributes", this);
    auto* formLayout    = new QFormLayout(attributesBox);

    m_startNodeEdit->setPlaceholderText("e.g. 0");
    m_endNodeEdit->setPlaceholderText("e.g. 5");

    formLayout->addRow("start node:", m_startNodeEdit);
    formLayout->addRow("end node:", m_endNodeEdit);

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

    m_startNodeEdit->setEnabled(needsStart);
    m_endNodeEdit->setEnabled(needsEnd);

    if(!needsStart) {
        m_startNodeEdit->clear();
        m_startNodeEdit->setPlaceholderText("no parameters required");
    } else {
        m_startNodeEdit->setPlaceholderText("e.g. 0");
    }

    if(!needsEnd) {
        m_endNodeEdit->clear();
        m_endNodeEdit->setPlaceholderText(needsStart ? "not required" : "no parameters required");
    } else {
        m_endNodeEdit->setPlaceholderText("e.g. 5");
    }
}
