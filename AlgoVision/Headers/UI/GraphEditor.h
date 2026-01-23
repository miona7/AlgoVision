#ifndef GRAPHEDITOR_H
#define GRAPHEDITOR_H

#include <QGraphicsView>
#include <QWidget>

#include "Graph.h"
#include "GraphScene.h"
#include "GraphController.h"

class QUndoStack;
class GraphEditTab;
class QLabel;

class GraphEditor : public QWidget {
    Q_OBJECT

public:
    explicit GraphEditor(QWidget* = nullptr);
    ~GraphEditor() override;

private slots:
    void onAddRequestTrigger();
    void onRemoveRequestTrigger();
    void onClearRequestTrigger();

private:
    QUndoStack*    m_undoStack       = nullptr;
    GraphEditTab*  m_editTab         = nullptr;
    QLabel*        m_leftPlaceholder = nullptr;
    QGraphicsView* m_view            = nullptr;
    GraphScene*    m_scene           = nullptr;
    GraphController* m_controller    = nullptr;
    // Graph*         m_graph           = nullptr;

    void clear() const;

    int m_dummyState = 0; // samo za test
};

#endif // GRAPHEDITOR_H
