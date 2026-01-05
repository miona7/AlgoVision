#include "GraphScene.h"
#include "Node.h"
#include "NodeItem.h"
#include <QGraphicsSceneMouseEvent>


GraphScene::GraphScene(QObject *parent)
    : QGraphicsScene(parent) {
    setSceneRect(0, 0, 3000, 3000);
}

void GraphScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    const auto clickPos = event->scenePos();
    QGraphicsItem* item = itemAt(clickPos, QTransform());

    if (!item) {
        addNode(clickPos);
        event->accept();
        return;
    }

    QGraphicsScene::mousePressEvent(event);
}

void GraphScene::addNode(QPointF position) {
    Node* nodeModel = new Node(1, position.x(), position.y());
    NodeItem* nodeItem = new NodeItem(nodeModel);
    addItem(nodeItem);
}
