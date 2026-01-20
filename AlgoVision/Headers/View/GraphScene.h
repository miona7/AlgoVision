#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <Graph.h>
#include <NodeItem.h>
#include <QGraphicsScene>

class GraphScene : public QGraphicsScene {
public:
    explicit GraphScene(Graph*, QObject* = nullptr);

    enum class State { ADD, REMOVE, EDIT };

    void setState(GraphScene::State);
    void resetScene();
    void clearScene();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent*) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;

private slots:
    void onNodeSelectTrigger(NodeItem*);
    void onEdgeSelectTrigger(EdgeItem*);

    // reacts to editabletextitem scene change
    void setEditGraphSceneTrigger(bool edit, EditableTextItem* label = nullptr);

private:
    NodeItem*         m_firstNodeSelect {nullptr};
    EditableTextItem* m_editLabel {nullptr};
    State             m_state {State::ADD};
    Graph*            m_graph {nullptr};

    void addNode(QPointF);
    void removeNode(NodeItem*);
    void selectNode(NodeItem*);
    void addEdge(NodeItem*, NodeItem*);
    void removeEdge(EdgeItem*);
    EdgeItem* makeEdgeItem(Edge*, NodeItem*, NodeItem*) const;
};

#endif // GRAPHSCENE_H
