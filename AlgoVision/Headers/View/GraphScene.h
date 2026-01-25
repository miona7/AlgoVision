#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <QGraphicsScene>

#include "NodeItem.h"
#include "Edge.h"

class GraphScene : public QGraphicsScene {
    Q_OBJECT

public:
    explicit GraphScene(QObject* = nullptr);

    enum class State { ADD, REMOVE, EDIT };

    void setState(GraphScene::State);
    void resetScene();

    void addNode(Node*);
    void addEdge(Edge*, NodeItem*, NodeItem*, bool, bool);
    void removeNode(NodeItem*);
    void removeEdge(EdgeItem*);

signals:
    void addNodeRequest(const QPointF&);
    void addEdgeRequest(NodeItem*, NodeItem*);
    void removeNodeRequest(NodeItem*);
    void removeEdgeRequest(EdgeItem*);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent*) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent*) override;

private slots:
    void onNodeSelectTrigger(NodeItem*);
    void onEdgeSelectTrigger(EdgeItem*);

    // reacts to editabletextitem scene change
    void setEditGraphSceneTrigger(bool, EditableTextItem* = nullptr);

private:
    NodeItem*         m_firstNodeSelect {nullptr};
    EditableTextItem* m_editLabel {nullptr};
    State             m_state {State::ADD};

    void      selectNode(NodeItem*);
    EdgeItem* makeEdgeItem(Edge*, NodeItem*, NodeItem*, bool, bool) const;
};

#endif // GRAPHSCENE_H
