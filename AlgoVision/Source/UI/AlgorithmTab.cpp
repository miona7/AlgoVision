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

AlgorithmTab::AlgorithmTab(std::shared_ptr<Graph> graph, QWidget* parent)
    : QWidget(parent), m_algorithmCombo(new QComboBox(this)), m_startRow(new QWidget(this)),
      m_startLabel(new QLabel("start node:", this)), m_startNodeEdit(new QLineEdit(this)),
      m_endRow(new QWidget(this)), m_endLabel(new QLabel("end node:", this)),
      m_endNodeEdit(new QLineEdit(this)), m_noInputLabel(new QLabel(this)),
      m_helpBtn(new QPushButton("help", this)), m_prevBtn(new QToolButton(this)),
      m_playBtn(new QToolButton(this)), m_pauseBtn(new QToolButton(this)),
      m_nextBtn(new QToolButton(this)), m_restartBtn(new QToolButton(this)), m_graph(graph),
      m_applier(m_graph), m_algorithmController(m_applier) {
    initLayout();
    initIcons();

    updateUiForAlgorithm(m_algorithmCombo->currentText());

    // Controller -> Tab (prikaz popup-a)
    connect(&m_algorithmController,
            &AlgorithmController::requestErrorDialog,
            this,
            &AlgorithmTab::showAlgorithmErrorDialog);

    // Tab -> Controller (odluka korisnika)
    connect(this, &AlgorithmTab::errorDialogCancelled, this, [this]() {
        // greska znaci da run NIJE uspeo → vracamo se u Idle
        m_state = RunState::Idle;

               // ponistavamo current config da sledeci Play uvek krene iznova
        m_currentConfig.reset();

               // sigurnosno: ugasi worker ako postoji
        if (m_worker != nullptr) {
            m_worker->quit();
            m_worker->wait();
            delete m_worker;
            m_worker = nullptr;
        }
    });

    m_algorithmController.clear(); // za svaki slucaj, da se ne zbuni kontroler nakon cancel

    connect(this, &AlgorithmTab::errorDialogContinue, this, [this]() {
        // ZA SAD SAMO RESET (bice prosireno)
        m_algorithmController.reset();
        m_state = RunState::Idle;
    });

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

    // povezivanje dugmica
    // moraju da se hvataju exepctioni -> iskacuci prozori?
    connect(m_playBtn, &QToolButton::clicked, this, [this]() {
        // parametri trenutnog algoritma
        AlgorithmTab::AlgorithmConfig newConfig = selectedConfig();

        // ako algoritam ili parametri nisu isti → NOVI START
        bool needNewRun = !m_currentConfig.has_value() || (newConfig != *m_currentConfig);

        if(needNewRun || m_state == RunState::Idle || m_state == RunState::Finished) {
            // ugasi i obrisi staru nit
            if(m_worker != nullptr) {
                m_worker->quit();
                m_worker->wait();
                delete m_worker;
                m_worker = nullptr;
            }

            m_algorithmController.reset(); // vrati graf u pocetno stanje
            m_currentConfig = newConfig;
            m_worker        = new AlgorithmWorker(newConfig.m_algorithmName, m_graph,
                                                  newConfig.m_startNode, newConfig.m_endNode);
            // Worker -> Controller (greske algoritma)
            connect(m_worker,
                    &AlgorithmWorker::algorithmErrorOccurred,
                    &m_algorithmController,
                    &AlgorithmController::onAlgorithmError);


            // pokreni iscrtavanje kad nit zavrsi
            connect(m_worker, &AlgorithmWorker::stepsReady, this, [this]() {
                m_state = RunState::Playing;
                startTimerForPlay();
            });

            // ucitaj korake algoritma
            connect(m_worker, &AlgorithmWorker::stepsReady, &m_algorithmController,
                    &AlgorithmController::loadSteps);

            m_worker->start();
            m_state = RunState::Playing;
            return;
        }

        //  RESUME –> isti algoritam, bio je pauziran
        if(m_state == RunState::Paused) {
            m_state = RunState::Playing;
            startTimerForPlay();
        }
    });

    connect(m_pauseBtn, &QToolButton::clicked, this, [this]() {
        if(m_timer != nullptr) {
            m_timer->stop();
        }
        m_state = RunState::Paused;
    });
    connect(m_nextBtn, &QToolButton::clicked, [this]() { m_algorithmController.nextStep(); });
    connect(m_prevBtn, &QToolButton::clicked, [this]() { m_algorithmController.prevStep(); });
    connect(m_restartBtn, &QToolButton::clicked, [this]() {
        m_algorithmController.reset();
        if(m_timer != nullptr) {
            m_timer->stop();
        }
        m_state = RunState::Idle;
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

    // separator 1
    auto* separator1 = new QFrame(this);
    separator1->setFrameShape(QFrame::HLine);
    separator1->setFrameShadow(QFrame::Sunken);
    mainLayout->addWidget(separator1);

    // run algorithm
    auto* runBox    = new QGroupBox("run algorithm", this);
    auto* runLayout = new QHBoxLayout(runBox);
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

    mainLayout->addWidget(runBox);

    // separator 2
    auto* separator2 = new QFrame(this);
    separator2->setFrameShape(QFrame::HLine);
    separator2->setFrameShadow(QFrame::Sunken);
    mainLayout->addWidget(separator2);

    // help
    m_helpBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    mainLayout->addWidget(m_helpBtn);

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
                            algorithmName == "A*";

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

bool AlgorithmTab::AlgorithmConfig::operator==(const AlgorithmConfig& other) const {
    return m_algorithmName == other.m_algorithmName && m_startNode == other.m_startNode &&
           m_endNode == other.m_endNode;
}

bool AlgorithmTab::AlgorithmConfig::operator!=(const AlgorithmConfig& other) const {
    return !(*this == other);
}

AlgorithmTab::AlgorithmConfig AlgorithmTab::selectedConfig() const {
    return {m_algorithmCombo->currentText(), m_startNodeEdit->text().toInt(),
            m_endNodeEdit->text().toInt()};
}

void AlgorithmTab::startTimerForPlay() {
    if(m_timer == nullptr) {
        m_timer = new QTimer(this);
        connect(m_timer, &QTimer::timeout, this, [this]() {
            if(m_algorithmController.isFinished()) {
                m_timer->stop();
                m_state = RunState::Finished;
                m_algorithmController.clear();
                return;
            }
            if(m_state != RunState::Playing) {
                return;
            }
            m_algorithmController.nextStep();
        });
    }
    m_timer->start(500); // 500ms po koraku
}

void AlgorithmTab::showAlgorithmErrorDialog(const AlgorithmError& error, bool allowContinue) {
    QMessageBox msgBox(this);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle("Algorithm error");
    msgBox.setText(QString::fromStdString(error.m_message));

    QPushButton* cancelBtn =
        msgBox.addButton("Cancel", QMessageBox::RejectRole);

    QPushButton* continueBtn = nullptr;
    if (allowContinue) {
        continueBtn =
            msgBox.addButton("Continue", QMessageBox::AcceptRole);
    }

    msgBox.exec();

    if (msgBox.clickedButton() == continueBtn) {
        emit errorDialogContinue();
    } else {
        emit errorDialogCancelled();
    }
}
