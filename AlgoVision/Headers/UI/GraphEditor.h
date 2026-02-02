#ifndef GRAPHEDITOR_H
#define GRAPHEDITOR_H

#include <QGraphicsView>
#include <QWidget>
#include <memory>

#include "Graph.h"
#include "GraphController.h"
#include "GraphScene.h"

class QUndoStack;
class GraphEditTab;
class QLabel;

class GraphEditor : public QWidget {
    Q_OBJECT

public:
    // kontroler mora da se postavi i da postoji pre graf editora, kako bi klase vezane za
    // algoritme koristile ispravan graf (koji nije nullptr)
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

private:
    QUndoStack*                      m_undoStack       = nullptr;
    GraphEditTab*                    m_editTab         = nullptr;
    QLabel*                          m_leftPlaceholder = nullptr;
    QGraphicsView*                   m_view            = nullptr;
    std::shared_ptr<GraphController> m_graphController;
};

#endif // GRAPHEDITOR_H
