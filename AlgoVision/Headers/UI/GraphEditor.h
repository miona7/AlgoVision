#ifndef GRAPHEDITOR_H
#define GRAPHEDITOR_H

#include <QGraphicsView>
#include <QWidget>
#include <memory>

#include "Graph.h"
#include "GraphScene.h"
#include "GraphController.h"

class QUndoStack;
class GraphEditTab;
class QLabel;

class GraphEditor : public QWidget {
    Q_OBJECT

public:
    explicit GraphEditor(const std::shared_ptr<GraphController>&, QWidget* = nullptr);
    ~GraphEditor() override;

    std::shared_ptr<GraphController> graphController() const;

private slots:
    void onAddRequestTrigger();
    void onRemoveRequestTrigger();
    void onClearRequestTrigger();

private:
    QUndoStack*    m_undoStack       = nullptr;
    GraphEditTab*  m_editTab         = nullptr;
    QLabel*        m_leftPlaceholder = nullptr;
    QGraphicsView* m_view            = nullptr;
    std::shared_ptr<GraphController> m_graphController;

    void clear() const;

    int m_dummyState = 0; // samo za test
};

#endif // GRAPHEDITOR_H
