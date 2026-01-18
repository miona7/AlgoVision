#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <QGraphicsScene>

#include "Graph.h"
#include "NodeItem.h"

class GraphScene : public QGraphicsScene {
public:
    explicit GraphScene(Graph*, QObject* = nullptr);

    enum class State { ADD, REMOVE };

    void setState(GraphScene::State);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent*) override;

private slots:
    void onNodeSelectTrigger(NodeItem*);
    void onEdgeSelectTrigger(EdgeItem*);

private:
    NodeItem* m_firstNodeSelect {nullptr};
    State     m_state {State::ADD};
    Graph*    m_graph {nullptr};

    void addNode(QPointF);
    void removeNode(NodeItem*);
    void selectNode(NodeItem*);
    void addEdge(NodeItem*, NodeItem*);
    void removeEdge(EdgeItem*);
};

#endif // GRAPHSCENE_H
