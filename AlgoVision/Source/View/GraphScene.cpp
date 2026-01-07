#include "GraphScene.h"
#include "Node.h"
#include "NodeItem.h"
#include <Edge.h>
#include <EdgeItem.h>
#include <QGraphicsSceneMouseEvent>
#include <UndirectedEdgeItem.h>


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

void GraphScene::onNodeSelectTrigger(NodeItem *node) {
    // node is selected
    if (m_firstNodeSelect == nullptr) {
        m_firstNodeSelect = node;
        return;
    }

    // same node is unselected
    if (m_firstNodeSelect == node) {
        m_firstNodeSelect = nullptr;
        return;
    }

    // other node is selected
    addEdge(m_firstNodeSelect, node);
}

void GraphScene::addNode(QPointF position) {
    Node* nodeModel = new Node(1, position.x(), position.y());
    NodeItem* nodeItem = new NodeItem(nodeModel);
    addItem(nodeItem);
    connect(nodeItem, &NodeItem::nodeSelected, this, &GraphScene::onNodeSelectTrigger);

    if (m_firstNodeSelect) {
        addEdge(m_firstNodeSelect, nodeItem);
    }
}

void GraphScene::addEdge(NodeItem *source, NodeItem *dest) {
    Edge* edgePlaceHolder = nullptr; // only for testing
    EdgeItem* edgeItem = new UndirectedEdgeItem(edgePlaceHolder, source, dest);
    addItem(edgeItem);

    source->setNodeSelected(false);
    dest->setNodeSelected(false);

    m_firstNodeSelect = nullptr;
}





