#include "AlgorithmTab.h"

AlgorithmTab::AlgorithmTab(std::shared_ptr<GraphController> graphController, QWidget* parent)
    : QWidget(parent), m_algorithmCombo(new QComboBox(this)), m_startRow(new QWidget(this)),
      m_startLabel(new QLabel("start node:", this)), m_startNodeEdit(new QLineEdit(this)),
      m_endRow(new QWidget(this)), m_endLabel(new QLabel("goal node:", this)),
      m_endNodeEdit(new QLineEdit(this)), m_noInputLabel(new QLabel(this)),
      m_helpBtn(new QPushButton("graph type help", this)), m_prevBtn(new QToolButton(this)),
      m_playBtn(new QToolButton(this)), m_pauseBtn(new QToolButton(this)),
      m_nextBtn(new QToolButton(this)), m_restartBtn(new QToolButton(this)),
      m_graphController(graphController), m_applier(m_graphController->graph()),
      m_algorithmController(m_applier) {

    m_legendContainer = new QWidget;
    m_legendLayout    = new QVBoxLayout(m_legendContainer);
    m_legendLayout->setAlignment(Qt::AlignTop);

    m_legendScroll = new QScrollArea(this);
    m_legendScroll->setWidget(m_legendContainer);
    m_legendScroll->setMinimumHeight(AppConstants::legendMinHeight);
    m_legendScroll->setWidgetResizable(true);

    initLayout();
    initIcons();
    updateControls();

    updateUiForAlgorithm(m_algorithmCombo->currentText());
    updateLegendForAlgorithm(m_algorithmCombo->currentText());

    // controller -> tab (popup)
    connect(&m_algorithmController, &AlgorithmController::requestErrorDialog, this,
            &AlgorithmTab::showAlgorithmErrorDialog);

    connect(m_algorithmCombo, &QComboBox::currentTextChanged, this,
            &AlgorithmTab::updateUiForAlgorithm);
    connect(m_algorithmCombo, &QComboBox::currentTextChanged, this,
            &AlgorithmTab::updateLegendForAlgorithm);

    // help
    connect(m_helpBtn, &QPushButton::clicked, this, [this]() {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Algorithm Tab Help");
        msgBox.setIcon(QMessageBox::Information);

        msgBox.setText(
            "<h3>Algorithm Tab — Help</h3>"

            "<p><b>Supported graph types by algorithm:</b></p>"

            "<ul>"
            "<li><b>BFS / DFS</b><br>"
            "Works on every graph.<br></li>"

            "<li><b>Dijkstra</b><br>"
            "Directed or undirected graphs.<br>"
            "Must be: weighted, with non-negative edge weights.<br></li>"

            "<li><b>Bellman–Ford</b><br>"
            "Negative edge weights allowed.<br>"
            "Must be: directed, weighted.<br>"
            "Note: detects negative cycles; shortest paths valid only if none are reachable from "
            "the source.<br></li>"

            "<li><b>Floyd–Warshall</b><br>"
            "Negative edge weights allowed.<br>"
            "Must be: directed, weighted.<br>"
            "Note: shortest paths are valid only if the graph contains no negative cycles.<br></li>"

            "<li><b>A* (Euclidean heuristic)</b><br>"
            "Directed or undirected graphs.<br>"
            "Must be: weighted, with non-negative edge weights.<br></li>"

            "<li><b>Prim</b><br>"
            "Must be: undirected, weighted, connected graph.<br></li>"

            "<li><b>Kahn</b><br>"
            "Must be: directed, acyclic graph (DAG).<br></li>"

            "<li><b>Tarjan</b><br>"
            "Must be: directed graph.<br></li>"
            "</ul>");

        msgBox.setTextFormat(Qt::RichText);
        msgBox.setStandardButtons(QMessageBox::Ok);

        msgBox.exec();
    });

    // connecting buttons
    connect(m_playBtn, &QToolButton::clicked, this, [this]() {
        m_playBtn->setEnabled(false);

        // params of current algorithm
        AlgorithmTab::AlgorithmConfig newConfig = selectedConfig();

        m_startNodeEdit->clear();
        m_endNodeEdit->clear();

        // new start
        bool needNewRun = !m_currentConfig.has_value() || (newConfig != *m_currentConfig);

        if(needNewRun || m_state == RunState::Idle || m_state == RunState::Finished) {

            m_applier.resetGraphState();
            m_algorithmController.clear();

            // reset legend
            updateLegendForAlgorithm(newConfig.m_algorithmName);

            // delete old thread
            if(m_worker != nullptr) {
                m_worker->quit();
                m_worker->wait();
                delete m_worker;
                m_worker = nullptr;
            }

            auto graph = m_graphController->graph();
            m_applier.setGraph(graph);

            m_currentConfig = newConfig;

            const auto& nodes = graph->getNodes();
            if(!m_startNodeEdit->text().isEmpty()) {
                if(nodes.find(newConfig.m_startNode) == nodes.end()) {
                    AlgorithmError err {AlgorithmErrorType::StartNodeMissing,
                                        "The specified start node does not exist in the graph."};
                    showAlgorithmErrorDialog(err);
                    return;
                }
            }

            if(!m_endNodeEdit->text().isEmpty()) {
                if(nodes.find(newConfig.m_endNode) == nodes.end()) {
                    AlgorithmError err {AlgorithmErrorType::GoalNodeMissing,
                                        "The specified end node does not exist in the graph."};
                    showAlgorithmErrorDialog(err);
                    return;
                }
            }

            m_worker = new AlgorithmWorker(newConfig.m_algorithmName, graph, newConfig.m_startNode,
                                           newConfig.m_endNode);

            // worker -> contoller (errors)
            connect(m_worker, &AlgorithmWorker::algorithmErrorOccurred, &m_algorithmController,
                    &AlgorithmController::onAlgorithmError);

            // start visuelization when thread is done
            connect(m_worker, &AlgorithmWorker::stepsReady, this, [this]() {
                m_state = RunState::Playing;
                startTimerForPlay();
                updateControls();
            });

            // load algortihm steps
            connect(m_worker, &AlgorithmWorker::stepsReady, &m_algorithmController,
                    &AlgorithmController::loadSteps);

            // load algorithm result
            connect(m_worker, &AlgorithmWorker::resultReady, &m_algorithmController,
                    &AlgorithmController::setResultString);

            m_worker->start();
            return;
        }

        // resume
        if(m_state == RunState::Paused) {
            m_state = RunState::Playing;
            startTimerForPlay();
            updateControls();
        }
    });

    connect(m_pauseBtn, &QToolButton::clicked, this, [this]() {
        if(m_timer != nullptr) {
            m_timer->stop();
        }
        m_state = RunState::Paused;
        updateControls();
    });

    connect(m_nextBtn, &QToolButton::clicked, [this]() { m_algorithmController.nextStep(); });

    connect(m_prevBtn, &QToolButton::clicked, [this]() { m_algorithmController.prevStep(); });

    connect(m_restartBtn, &QToolButton::clicked, this, [this]() {
        m_algorithmController.reset();
        if(m_timer != nullptr) {
            m_timer->stop();
        }
        m_state = RunState::Idle;
        updateControls();
        updateLegendForAlgorithm(m_algorithmCombo->currentText());
    });
}

void AlgorithmTab::initLayout() {
    auto* mainLayout = new QVBoxLayout(this);

    // algorithm choice
    auto* chooseAlgoBox    = new QGroupBox("choose algorithm", this);
    auto* chooseAlgoLayout = new QVBoxLayout(chooseAlgoBox);

    auto* model = qobject_cast<QStandardItemModel*>(m_algorithmCombo->model());

    auto addHeader = [&](const QString& title) {
        m_algorithmCombo->addItem(title);
        auto* item = model->item(model->rowCount() - 1);
        item->setFlags(Qt::NoItemFlags); // disabled / not selectable

        // make it look like a section header
        QFont f = item->font();
        f.setBold(true);
        item->setFont(f);

        item->setTextAlignment(Qt::AlignCenter);
    };

    addHeader("Graph traversal");
    m_algorithmCombo->addItem("BFS");
    m_algorithmCombo->addItem("DFS");

    addHeader("Shortest path");
    m_algorithmCombo->addItem("Dijkstra");
    m_algorithmCombo->addItem("Bellman-Ford");
    m_algorithmCombo->addItem("Floyd-Warshall");
    m_algorithmCombo->addItem("A* (Euclidean heuristic)");

    addHeader("Topological sort");
    m_algorithmCombo->addItem("Kahn");

    addHeader("Strongly connected components");
    m_algorithmCombo->addItem("Tarjan");

    addHeader("Spanning tree");
    m_algorithmCombo->addItem("Prim");

    // try to show the whole dropdown without scrolling
    m_algorithmCombo->setMaxVisibleItems(m_algorithmCombo->count());

    // try to reduce hover/selection visual effects on the popup list
    m_algorithmCombo->view()->setMouseTracking(false);
    m_algorithmCombo->view()->setStyleSheet("QListView::item:hover { background: transparent; }");

    chooseAlgoLayout->addWidget(m_algorithmCombo);
    mainLayout->addWidget(chooseAlgoBox);

    // set initial selection to the first enabled item (so headers never become initial selection)
    int firstValid = -1;
    if(model != nullptr) {
        for(int i = 0; i < model->rowCount(); i++) {
            QStandardItem* item = model->item(i);
            if(item && (item->flags() & Qt::ItemIsEnabled)) {
                firstValid = i;
                break;
            }
        }
    }
    if(firstValid >= 0) {
        m_algorithmCombo->setCurrentIndex(firstValid);
    }

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

    mainLayout->addWidget(m_legendScroll);
    mainLayout->addStretch();
}

QWidget* AlgorithmTab::makeLegendItem(const QColor& color, const QString& text) {
    QWidget* row    = new QWidget(this);
    auto*    layout = new QHBoxLayout(row);
    layout->setContentsMargins(2, 2, 2, 2);

    QLabel* box = new QLabel;
    box->setFixedSize(14, 14);
    box->setStyleSheet(QString("background-color: %1; border: 1px solid black;").arg(color.name()));

    QLabel* label = new QLabel(text);

    layout->addWidget(box);
    layout->addWidget(label);
    layout->addStretch();

    return row;
}

void AlgorithmTab::updateLegendForAlgorithm(const QString& name) {
    QLayoutItem* child;
    while((child = m_legendLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    QLabel* title = new QLabel("<b>Legend</b>");
    m_legendLayout->addWidget(title);

    m_legendLayout->addWidget(new QLabel("<b>Nodes</b>"));
    m_legendLayout->addWidget(makeLegendItem(Qt::yellow, "Active"));
    m_legendLayout->addWidget(makeLegendItem(Qt::blue, "Visited"));

    if(name == "Dijkstra" || name == "Bellman-Ford" || name == "A* (Euclidean heuristic)" ||
       name == "Floyd-Warshall" || name == "Prim" || name == "Tarjan") {
        m_legendLayout->addWidget(makeLegendItem(Qt::darkMagenta, "Distance updated"));
    }

    if(name == "A* (Euclidean heuristic)") {
        m_legendLayout->addWidget(makeLegendItem(Qt::green, "In final path"));
    }

    if(name == "Kahn") {
        m_legendLayout->addWidget(makeLegendItem(Qt::darkYellow, "Added to topological order"));
    }

    if(name == "Tarjan") {
        m_legendLayout->addWidget(new QLabel("Each component is colored differently."));
    }

    if(name != "Kahn") {
        m_legendLayout->addWidget(new QLabel("<b>Edges</b>"));
        m_legendLayout->addWidget(makeLegendItem(Qt::blue, "Examined"));
    }

    if(name == "Bellman-Ford") {
        m_legendLayout->addWidget(makeLegendItem(Qt::yellow, "Relaxed"));
    }

    if(name == "A* (Euclidean heuristic)") {
        m_legendLayout->addWidget(makeLegendItem(Qt::red, "In final path"));
    }

    if(name == "Prim") {
        m_legendLayout->addWidget(makeLegendItem(Qt::red, "In minimum spanning tree"));
    }
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
                            algorithmName == "A* (Euclidean heuristic)";

    const bool needsEnd = algorithmName == "A* (Euclidean heuristic)";

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
        m_startNodeEdit->setPlaceholderText("default 0");
    } else {
        m_startNodeEdit->clear();
        m_startRow->hide();
    }

    if(needsEnd) {
        m_endRow->show();
        m_endNodeEdit->setPlaceholderText("default 0");
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
                updateControls();

                QString result = m_algorithmController.resultString();
                if(!result.isEmpty()) {
                    m_legendLayout->addWidget(new QLabel("<b>Result</b>"));
                    m_legendLayout->addWidget(new QLabel(result));
                }

                m_algorithmController.clear();

                return;
            }
            if(m_state != RunState::Playing) {
                return;
            }
            m_algorithmController.nextStep();
        });
    }

    m_timer->start(500); // 500ms by step
}

void AlgorithmTab::updateControls() {
    // play is available only if we are already in move
    m_playBtn->setEnabled(m_state == RunState::Idle || m_state == RunState::Paused ||
                          m_state == RunState::Finished);

    // pause is available only if animation is in move
    m_pauseBtn->setEnabled(m_state == RunState::Playing);

    // stop (restart) is available only if we are not at the begining
    m_restartBtn->setEnabled(m_state != RunState::Idle);

    // next/prev is available only if state is paused
    m_nextBtn->setEnabled(m_state == RunState::Paused);
    m_prevBtn->setEnabled(m_state == RunState::Paused);

    // lock input while algorithm is running
    bool inputsLocked = (m_state == RunState::Playing || m_state == RunState::Paused);
    m_algorithmCombo->setEnabled(!inputsLocked);
    m_startNodeEdit->setEnabled(!inputsLocked);
    m_endNodeEdit->setEnabled(!inputsLocked);
}

void AlgorithmTab::showAlgorithmErrorDialog(const AlgorithmError& error) {

    QString userHint;

    switch(error.m_type) {
    case AlgorithmErrorType::GraphNotInitialized:
        userHint = "Please create a graph in the 'graph' tab before running an algorithm.";
        break;

    case AlgorithmErrorType::GraphTypeInvalid:
        userHint = "Please consult the 'graph type help' button to see which graph types are "
                   "supported.\n\n"
                   "Then, choose one of the following:\n"
                   "1) Click the 'create graph' button in the upper left corner to create a "
                   "suitable graph, "
                   "or clear the current graph from the scene in the 'graph' tab.\n"
                   "2) Choose a compatible algorithm and run it on the existing graph.";
        break;

    case AlgorithmErrorType::StartNodeMissing:
    case AlgorithmErrorType::GoalNodeMissing:
        userHint =
            "Please enter a valid node index or leave the field empty to use the default behavior.";
        break;

    case AlgorithmErrorType::NegativeEdgeWeights:
        userHint = "Please remove negative weights from the graph.";
        break;

    case AlgorithmErrorType::NoPathFound:
        userHint = "Please choose different nodes or modify the graph.";
        break;

    case AlgorithmErrorType::GraphHasNegativeCycle:
        userHint = "Please remove the cycle before running this algorithm.";
        break;

    case AlgorithmErrorType::GraphHasCycle:
        userHint = "Please modify the graph so it becomes acyclic.";
        break;

    case AlgorithmErrorType::GraphNotConnected:
        userHint = "Please ensure all nodes are reachable.";
        break;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Algorithm cannot be executed");
    dialog.setModal(true);

    auto* layout = new QVBoxLayout(&dialog);

    // algorithm message
    QLabel* mainText = new QLabel(QString::fromStdString(error.m_message));
    QFont   f        = mainText->font();
    f.setBold(true);
    mainText->setFont(f);
    mainText->setWordWrap(true);

    // user instructions
    QLabel* detailsText = new QLabel(userHint);
    detailsText->setWordWrap(true);

    layout->addWidget(mainText);
    layout->addSpacing(12);
    layout->addWidget(detailsText);
    layout->addStretch();

    QPushButton* okBtn = new QPushButton("OK");
    connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);

    layout->addWidget(okBtn, 0, Qt::AlignRight);

    dialog.resize(AppConstants::graphTypeDialogErrorMinWidth, dialog.sizeHint().height());
    dialog.setMinimumSize(AppConstants::graphTypeDialogErrorMinWidth,
                          AppConstants::graphTypeDialogErrorMinHeight);

    dialog.exec();

    m_state = RunState::Idle;
    updateControls();
}
