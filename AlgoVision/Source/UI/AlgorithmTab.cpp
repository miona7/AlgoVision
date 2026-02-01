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
#include <QStandardItem>
#include <QStandardItemModel>
#include <QAbstractItemView>

AlgorithmTab::AlgorithmTab(std::shared_ptr<Graph> graph, QWidget* parent)
    : QWidget(parent), m_algorithmCombo(new QComboBox(this)), m_startRow(new QWidget(this)),
      m_startLabel(new QLabel("start node:", this)), m_startNodeEdit(new QLineEdit(this)),
      m_endRow(new QWidget(this)), m_endLabel(new QLabel("end node:", this)),
      m_endNodeEdit(new QLineEdit(this)), m_noInputLabel(new QLabel(this)),
      m_helpBtn(new QPushButton("help", this)), m_prevBtn(new QToolButton(this)),
      m_playBtn(new QToolButton(this)), m_pauseBtn(new QToolButton(this)),
      m_nextBtn(new QToolButton(this)), m_restartBtn(new QToolButton(this)), m_graph(graph),
      m_applier(m_graph), m_algorithmController(m_applier) {

    m_legendContainer = new QWidget;
    m_legendLayout    = new QVBoxLayout(m_legendContainer);
    m_legendLayout->setAlignment(Qt::AlignTop);

    m_legendScroll = new QScrollArea(this);
    m_legendScroll->setWidget(m_legendContainer);
    m_legendScroll->setWidgetResizable(true);
    m_legendScroll->setMinimumHeight(200);

    m_legendScroll->setWidgetResizable(true);
    m_legendScroll->setMinimumHeight(200);

    initLayout();
    initIcons();

    updateUiForAlgorithm(m_algorithmCombo->currentText());
    updateLegendForAlgorithm(m_algorithmCombo->currentText());

    connect(m_algorithmCombo, &QComboBox::currentTextChanged, this,
            &AlgorithmTab::updateUiForAlgorithm);
    connect(m_algorithmCombo, &QComboBox::currentTextChanged, this,
            &AlgorithmTab::updateLegendForAlgorithm);

    // ===== HELP =====
    connect(m_helpBtn, &QPushButton::clicked, this, [this]() {
        QMessageBox::information(this, "Algorithm Tab Help",
                                 "Algorithm Tab — Help\n\n"
                                 "Choose algorithm, set parameters and use controls to run.");
    });

           // povezivanje dugmica
           // moraju da se hvataju exepctioni -> iskacuci prozori?
    connect(m_playBtn, &QToolButton::clicked, this, [this]() {
        // parametri trenutnog algoritma
        AlgorithmTab::AlgorithmConfig newConfig = selectedConfig();

               // ako algoritam ili parametri nisu isti → NOVI START
        bool needNewRun = !m_currentConfig.has_value() || (newConfig != *m_currentConfig);

        if(needNewRun || m_state == RunState::Idle || m_state == RunState::Finished) {

            // reset legend (da ne bude duplo)
            updateLegendForAlgorithm(newConfig.m_algorithmName);

            // ugasi i obrisi staru nit
            if(m_worker != nullptr) {
                m_worker->quit();
                m_worker->wait();
                delete m_worker;
                m_worker = nullptr;
            }

            m_algorithmController.reset(); // vrati graf u pocetno stanje
            m_currentConfig = newConfig;

            m_worker = new AlgorithmWorker(newConfig.m_algorithmName, m_graph,
                                           newConfig.m_startNode, newConfig.m_endNode);

                   // pokreni iscrtavanje kad nit zavrsi
            connect(m_worker, &AlgorithmWorker::stepsReady, this, [this]() {
                m_state = RunState::Playing;
                startTimerForPlay();
            });

                   // ucitaj korake algoritma
            connect(m_worker, &AlgorithmWorker::stepsReady, &m_algorithmController,
                    &AlgorithmController::loadSteps);

            // ucitaj rezultat algoritma
            connect(m_worker, &AlgorithmWorker::resultReady, &m_algorithmController,
                    &AlgorithmController::setResultString);

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

    // ===== PAUSE =====
    connect(m_pauseBtn, &QToolButton::clicked, this, [this]() {
        if(m_timer != nullptr) {
            m_timer->stop();
        }
        m_state = RunState::Paused;
    });

    connect(m_nextBtn, &QToolButton::clicked, [this]() { m_algorithmController.nextStep(); });

    connect(m_prevBtn, &QToolButton::clicked, [this]() { m_algorithmController.prevStep(); });

    connect(m_restartBtn, &QToolButton::clicked, this, [this]() {
        m_algorithmController.reset();
        if(m_timer != nullptr) {
            m_timer->stop();
        }
        m_state = RunState::Idle;
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
        //f.setUnderline(true);
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
    if(model) {
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

    if(name == "Dijkstra" || name == "Bellman-Ford" || name == "A*" || name == "Floyd-Warshall" ||
       name == "Prim") {
        m_legendLayout->addWidget(makeLegendItem(Qt::magenta, "Distance updated"));
    }

    if(name == "A*") {
        m_legendLayout->addWidget(makeLegendItem(Qt::green, "In final path"));
    }

    if(name == "Kahn") {
        m_legendLayout->addWidget(makeLegendItem(QColor(184, 134, 11), "Added to topological order"));
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

           // A* ima crvenu granu = u konačnoj putanji
    if(name == "A*") {
        m_legendLayout->addWidget(makeLegendItem(Qt::red, "In final path"));
    }

    if(name == "Prim") {
        m_legendLayout->addWidget(makeLegendItem(Qt::red, "In minimum spanning tree"));
    }

    //m_legendLayout->addWidget(new QLabel("<b>Result</b>"));
/*
    if(name == "Tarjan") {
        m_legendLayout->addWidget(new QLabel("Strongly connected components."));
    }
*/
    if(name == "Kahn") {
        m_legendLayout->addWidget(new QLabel("Topological ordering."));
    }

    if(name == "BFS" || name == "DFS") {
        m_legendLayout->addWidget(new QLabel("Traversal order."));
    }

    m_legendLayout->addStretch();
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

    m_timer->start(500); // 500ms po koraku

    if(m_algorithmController.isFinished()) {
        m_timer->stop();
        m_state = RunState::Finished;

        QString result = m_algorithmController.resultString();
        if(!result.isEmpty()) {
            m_legendLayout->addWidget(new QLabel("<b>Result</b>"));
            m_legendLayout->addWidget(new QLabel(result));
        }
        m_algorithmController.clear();
        return;
    }
}
