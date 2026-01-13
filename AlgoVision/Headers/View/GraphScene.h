#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <NodeItem.h>
#include <QGraphicsScene>

class GraphScene : public QGraphicsScene {
public:
    explicit GraphScene(QObject* parent = nullptr);

    enum class State { ADD, REMOVE };

    void setState(GraphScene::State state);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private slots:
    void onNodeSelectTrigger(NodeItem* node);
    void onEdgeSelectTrigger(EdgeItem* edge);

private:
    void addNode(QPointF position);
    void removeNode(NodeItem* node);
    void selectNode(NodeItem* node);
    void addEdge(NodeItem* source, NodeItem* dest);
    void removeEdge(EdgeItem* edge);

    NodeItem* m_firstNodeSelect {nullptr};
    State     m_state {State::ADD};
};

#endif // GRAPHSCENE_H
