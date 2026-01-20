#include <QGraphicsSceneMouseEvent>

#include "DirectedEdgeItem.h"
#include "Edge.h"
#include "EdgeItem.h"
#include "GraphScene.h"
#include "EditableTextItem.h"
#include "Node.h"
#include "NodeItem.h"
#include "UndirectedEdgeItem.h"

GraphScene::GraphScene(Graph* graph, QObject* parent) : m_graph(graph), QGraphicsScene(parent) {
    setSceneRect(0, 0, 3000, 3000);
}

void GraphScene::setState(GraphScene::State state) {
    m_state = state;
}

void GraphScene::resetScene() {
    if(m_firstNodeSelect) {
        m_firstNodeSelect->setNodeSelected(false);
        m_firstNodeSelect = nullptr;
    }

    if(m_editLabel) {
        m_editLabel->finishEditing(false);
        m_editLabel = nullptr;
    }

    m_state = State::ADD;
}

void GraphScene::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if(m_state == State::EDIT) {
        m_editLabel->finishEditing(true);
        event->accept();
        return;
    }

    const auto     clickPos = event->scenePos();
    QGraphicsItem* item     = itemAt(clickPos, QTransform());

    if(!item && m_state == State::ADD) {
        addNode(clickPos);
        event->accept();
        return;
    }

    QGraphicsScene::mousePressEvent(event);
}

void GraphScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) {
    if(m_state == State::REMOVE || m_state == State::EDIT) {
        event->accept();
        return;
    }

    QGraphicsScene::mouseDoubleClickEvent(event);
}

void GraphScene::clearScene() {
    clear(); // prvo brisemo sve UI iteme + observere
    m_graph->clear(); // onda brisemo model
}

void GraphScene::onNodeSelectTrigger(NodeItem* node) {
    if(m_state == State::ADD || m_state == State::EDIT) {
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

void GraphScene::setEditGraphSceneTrigger(bool edit, EditableTextItem* label) {
    if(edit) {
        m_state     = State::EDIT;
        m_editLabel = label;
    } else {
        m_state     = State::ADD;
        m_editLabel = nullptr;
    }
}

void GraphScene::addNode(QPointF position) {
    Node*     nodeModel = m_graph->addNode(position.x(), position.y());
    NodeItem* nodeItem  = new NodeItem(nodeModel);
    addItem(nodeItem);
    connect(nodeItem, &NodeItem::nodeSelected, this, &GraphScene::onNodeSelectTrigger);
    connect(nodeItem->label(), &EditableTextItem::setEditGraphSceneState, this,
            &GraphScene::setEditGraphSceneTrigger);

    if(m_firstNodeSelect) {
        addEdge(m_firstNodeSelect, nodeItem);
    }
}

void GraphScene::addEdge(NodeItem* source, NodeItem* dest) {
    unsigned sourceId = source->modelNode()->getId();
    unsigned destId   = dest->modelNode()->getId();
    m_graph->addEdge(sourceId, destId);

    Edge*     edgeModel = m_graph->getEdge(sourceId, destId);
    EdgeItem* edgeItem  = makeEdgeItem(edgeModel, source, dest);
    connect(edgeItem->weight(), &EditableTextItem::setEditGraphSceneState,
            this, &GraphScene::setEditGraphSceneTrigger);

    addItem(edgeItem);
    connect(edgeItem, &EdgeItem::edgeSelected, this, &GraphScene::onEdgeSelectTrigger);

    source->setNodeSelected(false);
    dest->setNodeSelected(false);
    m_firstNodeSelect = nullptr;
}

void GraphScene::removeEdge(EdgeItem* edge) {
    // m_graph->removeEdge(edge->modelEdge()->getId());
    // delete edge;

    const unsigned edgeId = edge->modelEdge()->getId();

    removeItem(edge); // prvo ukloni item sa scene, da bi mogao bezbedno da se obrise
    delete edge; // onda ukloni UI item + observer
    m_graph->removeEdge(edgeId); // onda ukloni model
}

EdgeItem* GraphScene::makeEdgeItem(Edge* modelEdge, NodeItem* src, NodeItem* dest) const {
    EdgeItem* edgeItem = nullptr;
    bool isWeighted = m_graph->isWeighted();

    if (m_graph->isDirected()) {
        edgeItem = new DirectedEdgeItem(modelEdge, src, dest, isWeighted);
    } else {
        edgeItem = new UndirectedEdgeItem(modelEdge, src, dest, isWeighted);
    }

    if (isWeighted) {
        edgeItem->initEdgeWeight();
    }

    edgeItem->adjust();

    return edgeItem;
}

void GraphScene::removeNode(NodeItem* node) {
    // m_graph->removeNode(node->modelNode()->getId());
    // delete node;

    const unsigned nodeId = node->modelNode()->getId();

    removeItem(node); // prvo ukloni item sa scene, da bi mogao bezbedno da se obrise
    delete node;                 // onda UI + observer
    m_graph->removeNode(nodeId); // onda model
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
