#include <DirectedEdgeItem.h>
#include <Edge.h>
#include <EdgeItem.h>
#include <GraphScene.h>
#include <Node.h>
#include <NodeItem.h>
#include <QGraphicsSceneMouseEvent>
#include <UndirectedEdgeItem.h>

GraphScene::GraphScene(Graph* graph, QObject* parent) : m_graph(graph), QGraphicsScene(parent) {
    setSceneRect(0, 0, 3000, 3000);
}

void GraphScene::setState(GraphScene::State state) {
    m_state = state;
}

void GraphScene::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    const auto     clickPos = event->scenePos();
    QGraphicsItem* item     = itemAt(clickPos, QTransform());

    if(!item && m_state == State::ADD) {
        addNode(clickPos);
        event->accept();
        return;
    }

    QGraphicsScene::mousePressEvent(event);
}

void GraphScene::onNodeSelectTrigger(NodeItem* node) {
    if(m_state == State::ADD) {
        selectNode(node);
    }

    if(m_state == State::REMOVE) {
        removeNode(node);
    }
}

void GraphScene::onEdgeSelectTrigger(EdgeItem* edge) {
    if(m_state == State::REMOVE) {
        removeEdge(edge);
    }
}

void GraphScene::addNode(QPointF position) {
    Node*     nodeModel = m_graph->addNode(position.x(), position.y());
    NodeItem* nodeItem  = new NodeItem(nodeModel);
    addItem(nodeItem);
    connect(nodeItem, &NodeItem::nodeSelected, this, &GraphScene::onNodeSelectTrigger);

    if(m_firstNodeSelect) {
        addEdge(m_firstNodeSelect, nodeItem);
    }
}

void GraphScene::addEdge(NodeItem* source, NodeItem* dest) {
    unsigned sourceId = source->modelNode()->getId();
    unsigned destId   = dest->modelNode()->getId();
    m_graph->addEdge(sourceId, destId);

    Edge*     edgeModel = m_graph->getEdge(sourceId, destId);
    EdgeItem* edgeItem  = nullptr;

    if(m_graph->isDirected()) {
        edgeItem = new DirectedEdgeItem(edgeModel, source, dest);
    } else {
        edgeItem = new UndirectedEdgeItem(edgeModel, source, dest);
    }

    addItem(edgeItem);
    connect(edgeItem, &EdgeItem::edgeSelected, this, &GraphScene::onEdgeSelectTrigger);

    source->setNodeSelected(false);
    dest->setNodeSelected(false);
    m_firstNodeSelect = nullptr;
}

void GraphScene::removeEdge(EdgeItem* edge) {
    m_graph->removeEdge(edge->modelEdge()->getId());
    delete edge;
}

void GraphScene::removeNode(NodeItem* node) {
    //m_graph->removeNode(node->modelNode()->getId());
    delete node;
}

void GraphScene::selectNode(NodeItem* node) {
    // node is selected
    if(m_firstNodeSelect == nullptr) {
        m_firstNodeSelect = node;
        return;
    }

    // same node is unselected
    if(m_firstNodeSelect == node) {
        m_firstNodeSelect = nullptr;
        return;
    }

    // other node is selected
    addEdge(m_firstNodeSelect, node);
}
