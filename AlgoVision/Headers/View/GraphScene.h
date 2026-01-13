#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <NodeItem.h>
#include <QGraphicsScene>

class GraphScene : public QGraphicsScene {
public:
    explicit GraphScene(QObject* parent = nullptr);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private slots:
    void onNodeSelectTrigger(NodeItem* node);

private:
    void      addNode(QPointF position);
    void      addEdge(NodeItem* source, NodeItem* dest);
    NodeItem* m_firstNodeSelect {nullptr};
};

#endif // GRAPHSCENE_H
