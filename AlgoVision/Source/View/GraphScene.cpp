#include <QGraphicsSceneMouseEvent>

#include "DirectedEdgeItem.h"
#include "Edge.h"
#include "EdgeItem.h"
#include "EditableTextItem.h"
#include "GraphScene.h"
#include "Node.h"
#include "NodeItem.h"
#include "UndirectedEdgeItem.h"

GraphScene::GraphScene(const std::shared_ptr<Graph> &graph, QObject *parent)
    : QGraphicsScene(parent), m_graph(graph) {
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
        emit addNodeRequest(clickPos); // zahtevamo dodavanje cvora od kontrolera
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

void GraphScene::onNodeSelectTrigger(NodeItem* node) {
    if(m_state == State::ADD || m_state == State::EDIT) {
        selectNode(node);
    }

    if(m_state == State::REMOVE) {
        emit removeNodeRequest(node); // zahtevamo brisanje cvora od kontrolera
    }
}

void GraphScene::onEdgeSelectTrigger(EdgeItem* edge) {
    if(m_state == State::REMOVE) {
        emit removeEdgeRequest(edge); // zahtevamo brisanje grane od kontrolera
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

// VIEW region
void GraphScene::addNode(Node* nodeModel) {
    NodeItem* nodeItem  = new NodeItem(nodeModel);
    addItem(nodeItem);
    connect(nodeItem, &NodeItem::nodeSelected, this, &GraphScene::onNodeSelectTrigger);
    connect(nodeItem->label(), &EditableTextItem::setEditGraphSceneState, this,
            &GraphScene::setEditGraphSceneTrigger);

    if(m_firstNodeSelect) {
        emit addEdgeRequest(m_firstNodeSelect, nodeItem); // zahtevamo dodavanje grane od kontrolera
    }
}

void GraphScene::addEdge(Edge* edgeModel, NodeItem* source, NodeItem* dest,
                         bool isDirected, bool isWeighted) {
    EdgeItem* edgeItem  = makeEdgeItem(edgeModel, source, dest, isDirected, isWeighted);
    addItem(edgeItem);
    connect(edgeItem, &EdgeItem::edgeSelected, this, &GraphScene::onEdgeSelectTrigger);

    source->setNodeSelected(false);
    dest->setNodeSelected(false);
    m_firstNodeSelect = nullptr;
}

EdgeItem* GraphScene::makeEdgeItem(Edge* modelEdge, NodeItem* src, NodeItem* dest,
                                   bool isDirected, bool isWeighted) const {
    EdgeItem* edgeItem   = nullptr;

    if(isDirected) {
        edgeItem = new DirectedEdgeItem(modelEdge, src, dest, isWeighted);
    } else {
        edgeItem = new UndirectedEdgeItem(modelEdge, src, dest, isWeighted);
    }

    if(isWeighted) {
        edgeItem->initEdgeWeight();
        connect(edgeItem->weight(), &EditableTextItem::setEditGraphSceneState, this,
                &GraphScene::setEditGraphSceneTrigger);
    }

    edgeItem->adjust();

    return edgeItem;
}

void GraphScene::removeNode(NodeItem* node) {
    removeItem(node);
    delete node;
}

void GraphScene::removeEdge(EdgeItem* edge) {
    removeItem(edge);
    delete edge;
}
// VIEW

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
    emit addEdgeRequest(m_firstNodeSelect, node); // zahtevamo dodavanje grane od kontrolera
}

// vraca raw pointer (postojeci)
Graph* GraphScene::getGraphRaw() const {
    return m_graph.get();
}

// pravi shared_ptr za algoritme, ne preuzima vlasnistvo
std::shared_ptr<Graph> GraphScene::getGraphShared() const {
    return m_graph;
}
