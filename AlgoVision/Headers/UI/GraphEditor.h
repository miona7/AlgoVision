#ifndef GRAPHEDITOR_H
#define GRAPHEDITOR_H

#include <QHBoxLayout>
#include <QKeySequence>
#include <QLabel>
#include <QShortcut>
#include <QSlider>
#include <QSplitter>
#include <QTabWidget>
#include <QUndoCommand>
#include <QUndoStack>
#include <QWidget>

#include <memory>
#include <functional>

#include "Graph.h"
#include "GraphView.h"
#include "GraphScene.h"
#include "AlgorithmTab.h"
#include "GraphEditTab.h"
#include "GraphController.h"

class GraphEditor : public QWidget {
    Q_OBJECT
public:
    explicit GraphEditor(const std::shared_ptr<GraphController>&, QWidget* = nullptr);
    ~GraphEditor() override = default;

    std::shared_ptr<GraphController> graphController() const;

signals:
    void undoRequested();
    void redoRequested();

private slots:
    void onAddRequestTrigger();
    void onRemoveRequestTrigger();
    void onClearRequestTrigger();
    void onUndoRequestTrigger();
    void onRedoRequestTrigger();
    void onPanRequestTrigger();
    void onZoomInRequestTrigger();
    void onZoomOutRequestTrigger();

private:
    QUndoStack*                      m_undoStack{nullptr};
    GraphEditTab*                    m_editTab{nullptr};
    QLabel*                          m_leftPlaceholder{nullptr};
    GraphView*                       m_view{nullptr};
    std::shared_ptr<GraphController> m_graphController;
};

#endif // GRAPHEDITOR_H
