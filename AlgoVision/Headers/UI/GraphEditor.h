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

#include <functional>
#include <memory>

#include "AlgorithmTab.h"
#include "Graph.h"
#include "GraphController.h"
#include "GraphEditTab.h"
#include "GraphScene.h"
#include "GraphView.h"

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
