#include <QHBoxLayout>
#include <QKeySequence>
#include <QLabel>
#include <QShortcut>
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

GraphEditor::GraphEditor(const std::shared_ptr<GraphController>& graphController, QWidget* parent)
    : m_graphController(graphController), QWidget(parent) {

    // m_graph = new UnweightedDirectedGraph();

    m_undoStack = m_graphController->undoStack();

    // Ctrl+Z
    auto* undoSc = new QShortcut(QKeySequence::Undo, this);
    connect(undoSc, &QShortcut::activated, m_undoStack, &QUndoStack::undo);

    // Ctrl+Y
    auto* redoCtrlY = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Y), this);
    connect(redoCtrlY, &QShortcut::activated, m_undoStack, &QUndoStack::redo);

    // main splitter for the left and right page sides
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);

    m_view = new QGraphicsView(splitter);
    m_view->setScene(m_graphController->scene());
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
}

GraphEditor::~GraphEditor() {
    // m_graphController->clear();
    m_graphController->clearNoHistory();
}

void GraphEditor::onAddRequestTrigger() {
    m_graphController->setAddSceneState();
}

void GraphEditor::onRemoveRequestTrigger() {
    m_graphController->setRemoveSceneState();
}

void GraphEditor::onClearRequestTrigger() {
    m_graphController->clear();
}

std::shared_ptr<GraphController> GraphEditor::graphController() const {
    return m_graphController;
}
