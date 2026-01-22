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

GraphEditor::GraphEditor(QWidget* parent) : QWidget(parent) {

    m_graph = new WeightedDirectedGraph();

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

    m_scene = new GraphScene(m_graph, splitter);
    m_view  = new QGraphicsView(splitter);
    m_view->setScene(m_scene);
    splitter->addWidget(m_view);

    // right side
    QTabWidget* rightTabs = new QTabWidget(splitter);

    // rightTabs->addTab(new GraphEditTab(rightTabs), "graph edit");
    m_editTab = new GraphEditTab(rightTabs);

    rightTabs->addTab(m_editTab, "graph");
    rightTabs->addTab(new AlgorithmTab(m_scene->getGraphShared(), rightTabs), "algorithm");

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
    clear();
    delete m_graph;
}

void GraphEditor::onAddRequestTrigger() {
    m_scene->resetScene();
    m_scene->setState(GraphScene::State::ADD);
}

void GraphEditor::onRemoveRequestTrigger() {
    m_scene->resetScene();
    m_scene->setState(GraphScene::State::REMOVE);
}

void GraphEditor::onClearRequestTrigger() {
    clear();
}

void GraphEditor::clear() const {
    m_scene->resetScene();
    m_scene->clearScene();
}
