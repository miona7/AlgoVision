#include <QHBoxLayout>
#include <QLabel>
#include <QSplitter>
#include <QTabWidget>
#include <QUndoCommand>
#include <QUndoStack>

#include <functional>

#include "AlgorithmTab.h"
#include "GraphEditTab.h"
#include "GraphEditor.h"
#include "GraphView.h"
#include "UnweightedDirectedGraph.h"
#include "UnweightedUndirectedGraph.h"

namespace {
    class LambdaCommand : public QUndoCommand {
    public:
        LambdaCommand(std::function<void()> redoFn, std::function<void()> undoFn,
                      const QString& text = "")
            : m_redo(std::move(redoFn)), m_undo(std::move(undoFn)) {
            setText(text);
        }

        void redo() override {
            if(m_redo) {
                m_redo();
            }
        }
        void undo() override {
            if(m_undo) {
                m_undo();
            }
        }

    private:
        std::function<void()> m_redo;
        std::function<void()> m_undo;
    };
} // namespace

GraphEditor::GraphEditor(const std::shared_ptr<GraphController>& graphController, QWidget* parent)
    : m_graphController(graphController), QWidget(parent) {

    // m_graph = new UnweightedDirectedGraph();

    m_undoStack = new QUndoStack(this);

    // main splitter for the left and right page sides
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);

    // left side (placeholder)
    // QLabel* leftPlaceholder = new QLabel("GRAPH / SCENE AREA", splitter);
    // leftPlaceholder->setAlignment(Qt::AlignCenter);
    // leftPlaceholder->setStyleSheet("background-color: #2b2b2b; color: white;");

    // m_leftPlaceholder = new QLabel("GRAPH / SCENE AREA", splitter);
    // m_leftPlaceholder->setAlignment(Qt::AlignCenter);
    // m_leftPlaceholder->setStyleSheet("background-color: #2b2b2b; color: white;");
    // splitter->addWidget(m_leftPlaceholder);

    // stari pogled (QGraphicsView)
    // m_view = new QGraphicsView(splitter);
    // m_view->setScene(m_graphController->scene());
    // splitter->addWidget(m_view);

    m_view = new GraphView(m_graphController->scene(), this);
    splitter->addWidget(m_view);

    // m_graphController = new GraphController(m_scene, this);

    // right side
    QTabWidget* rightTabs = new QTabWidget(splitter);

    // rightTabs->addTab(new GraphEditTab(rightTabs), "graph edit");
    m_editTab = new GraphEditTab(rightTabs);

    rightTabs->addTab(m_editTab, "graph");
    rightTabs->addTab(new AlgorithmTab(m_graphController->graph(), rightTabs), "algorithm");

    splitter->addWidget(rightTabs);

    // 80% left, 20% right
    splitter->setStretchFactor(0, 8);
    splitter->setStretchFactor(1, 2);

    // layout for the whole GraphEditor
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(splitter);

    connect(m_editTab, &GraphEditTab::undoRequested, m_undoStack, &QUndoStack::undo);

    connect(m_editTab, &GraphEditTab::redoRequested, m_undoStack, &QUndoStack::redo);

    connect(m_undoStack, &QUndoStack::canUndoChanged, m_editTab, &GraphEditTab::setUndoEnabled);

    connect(m_undoStack, &QUndoStack::canRedoChanged, m_editTab, &GraphEditTab::setRedoEnabled);

    connect(m_editTab, &GraphEditTab::addRequested, this, &GraphEditor::onAddRequestTrigger);

    connect(m_editTab, &GraphEditTab::removeRequested, this, &GraphEditor::onRemoveRequestTrigger);

    connect(m_editTab, &GraphEditTab::clearRequested, this, &GraphEditor::onClearRequestTrigger);

    connect(m_editTab, &GraphEditTab::panRequested, this, &GraphEditor::onPanRequestTrigger);

    connect(m_editTab, &GraphEditTab::zoomInRequested, m_view, &GraphView::zoomIn);

    connect(m_editTab, &GraphEditTab::zoomOutRequested, m_view, &GraphView::zoomOut);
    // Dummy test
    // connect(m_editTab, &GraphEditTab::addRequested, this, [this]() {
    //     const int before = m_dummyState;
    //     const int after  = before + 1;

    //     m_undoStack->push(new LambdaCommand(
    //         [this, after]() {
    //             m_dummyState = after;
    //             m_leftPlaceholder->setText(QString("dummy state: %1").arg(m_dummyState));
    //         },
    //         [this, before]() {
    //             m_dummyState = before;
    //             m_leftPlaceholder->setText(QString("dummy state: %1").arg(m_dummyState));
    //         },
    //         "Add dummy"));
    // });
}

GraphEditor::~GraphEditor() {
    m_graphController->clear();
}

void GraphEditor::onAddRequestTrigger() {
    m_view->resetState();
    m_graphController->setAddSceneState();
}

void GraphEditor::onRemoveRequestTrigger() {
    m_view->resetState();
    m_graphController->setRemoveSceneState();
}

void GraphEditor::onClearRequestTrigger() {
    m_view->resetState();
    m_graphController->clear();
}

void GraphEditor::onPanRequestTrigger() {
    // resetuj stanje scene na podrazumevano
    m_graphController->scene()->resetScene();
    m_view->setState(GraphView::State::PAN_IDLE);
}

void GraphEditor::onZoomInRequestTrigger() {
    // resetuj stanje scene na podrazumevano
    m_graphController->scene()->resetScene();
    m_view->setState(GraphView::State::ZOOM_IN);
}

void GraphEditor::onZoomOutRequestTrigger() {
    // resetuj stanje scene na podrazumevano
    m_graphController->scene()->resetScene();
    m_view->setState(GraphView::State::ZOOM_OUT);
}

std::shared_ptr<GraphController> GraphEditor::graphController() const {
    return m_graphController;
}
