#include <QHBoxLayout>
#include <QKeySequence>
#include <QLabel>
#include <QShortcut>
#include <QSplitter>
#include <QTabWidget>
#include <QUndoCommand>
#include <QUndoStack>

#include <QSlider>
#include <functional>

#include "AlgorithmTab.h"
#include "GraphEditTab.h"
#include "GraphEditor.h"
#include "GraphView.h"

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

    m_undoStack = m_graphController->undoStack();

    // Ctrl+Z
    auto* undoSc = new QShortcut(QKeySequence::Undo, this);
    connect(undoSc, &QShortcut::activated, m_undoStack, &QUndoStack::undo);

    // Ctrl+Y
    auto* redoCtrlY = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Y), this);
    connect(redoCtrlY, &QShortcut::activated, m_undoStack, &QUndoStack::redo);

    // main splitter for the left and right page sides
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);

    m_view = new GraphView(m_graphController->scene(), this);
    splitter->addWidget(m_view);

    // right side
    QTabWidget* rightTabs = new QTabWidget(splitter);

    // rightTabs->addTab(new GraphEditTab(rightTabs), "graph edit");
    m_editTab = new GraphEditTab(rightTabs);

    rightTabs->addTab(m_editTab, "graph");
    rightTabs->addTab(new AlgorithmTab(m_graphController, rightTabs), "algorithm");

    splitter->addWidget(rightTabs);

    // 80% left, 20% right
    splitter->setStretchFactor(0, 8);
    splitter->setStretchFactor(1, 2);

    // layout for the whole GraphEditor
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(splitter);
    // keyboard shortcuts

    // pan: Ctrl + P
    QShortcut* panShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_P), this);
    connect(panShortcut, &QShortcut::activated, this, &GraphEditor::onPanRequestTrigger);

    // zoom in: Ctrl + '+'
    QShortcut* zoomInShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Plus), this);
    connect(zoomInShortcut, &QShortcut::activated, this, &GraphEditor::onZoomInRequestTrigger);

    // zoom out: Ctrl + '-'
    QShortcut* zoomOutShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Minus), this);
    connect(zoomOutShortcut, &QShortcut::activated, this, &GraphEditor::onZoomOutRequestTrigger);

    //connect(m_editTab, &GraphEditTab::undoRequested, m_undoStack, &QUndoStack::undo);

    connect(m_editTab, &GraphEditTab::undoRequested, this, &GraphEditor::onUndoRequestTrigger);

    connect(m_editTab, &GraphEditTab::redoRequested, this, &GraphEditor::onRedoRequestTrigger);

    connect(this, &GraphEditor::undoRequested, m_undoStack, &QUndoStack::undo);

    connect(this, &GraphEditor::redoRequested, m_undoStack, &QUndoStack::redo);

    connect(m_undoStack, &QUndoStack::canUndoChanged, m_editTab, &GraphEditTab::setUndoEnabled);

    connect(m_undoStack, &QUndoStack::canRedoChanged, m_editTab, &GraphEditTab::setRedoEnabled);

    connect(m_editTab, &GraphEditTab::addRequested, this, &GraphEditor::onAddRequestTrigger);

    connect(m_editTab, &GraphEditTab::removeRequested, this, &GraphEditor::onRemoveRequestTrigger);

    connect(m_editTab, &GraphEditTab::clearRequested, this, &GraphEditor::onClearRequestTrigger);

    connect(m_editTab->getNodeSizeSlider(), &QSlider::valueChanged, this, [&](int v) {
        double t                = v / 100.0;
        AppConstants::NodeScale = AppConstants::MinNodeScale +
                                  t * (AppConstants::MaxNodeScale - AppConstants::MinNodeScale);

        m_graphController->scene()->updateNodeScalling();
    });

    connect(m_editTab, &GraphEditTab::panRequested, this, &GraphEditor::onPanRequestTrigger);

    connect(m_editTab, &GraphEditTab::zoomInRequested, this, &GraphEditor::onZoomInRequestTrigger);

    connect(m_editTab, &GraphEditTab::zoomOutRequested, this,
            &GraphEditor::onZoomOutRequestTrigger);

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

void GraphEditor::onUndoRequestTrigger() {
    m_graphController->resetScene();
    emit undoRequested();
}

void GraphEditor::onRedoRequestTrigger() {
    m_graphController->resetScene();
    emit redoRequested();
}

void GraphEditor::onPanRequestTrigger() {
    m_graphController->resetScene();
    m_view->setState(GraphView::State::PAN_IDLE);
}

void GraphEditor::onZoomInRequestTrigger() {
    m_graphController->resetScene();
    m_view->zoomIn();
}

void GraphEditor::onZoomOutRequestTrigger() {
    m_graphController->resetScene();
    m_view->zoomOut();
}

std::shared_ptr<GraphController> GraphEditor::graphController() const {
    return m_graphController;
}
