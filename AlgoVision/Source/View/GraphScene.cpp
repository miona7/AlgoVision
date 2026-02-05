#include "GraphScene.h"

GraphScene::GraphScene(QObject* parent) : QGraphicsScene(parent) {
    setSceneRect(0, 0, 3000, 3000);
}

void GraphScene::setState(GraphScene::State state) {
    m_state = state;
}

void GraphScene::resetScene() {
    if(m_firstNodeSelect != nullptr) {
        m_firstNodeSelect->setNodeSelected(false);
        m_firstNodeSelect = nullptr;
    }

    if(m_editLabel != nullptr) {
        m_editLabel->finishEditing(false);
        m_editLabel = nullptr;
    }
}

void GraphScene::disableScene() {
    m_previousState = m_state;
    m_state         = GraphScene::State::IDLE;
}

void GraphScene::enableScene() {
    m_state = m_previousState;
}

void GraphScene::clear() {
    resetScene();
    m_nodeItems.clear();
    QGraphicsScene::clear();
}

void GraphScene::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if(m_state == GraphScene::State::IDLE) {
        event->accept();
        return;
    }

    if(m_state == GraphScene::State::EDIT) {
        m_editLabel->finishEditing(true);
        event->accept();
        return;
    }

    const auto     clickPos = event->scenePos();
    QGraphicsItem* item     = itemAt(clickPos, QTransform());

    if(item == nullptr && m_state == GraphScene::State::ADD) {
        if(m_firstNodeSelect != nullptr) {
            emit addNodeAndEdgeRequest(clickPos, m_firstNodeSelect);
        } else {
            emit addNodeRequest(clickPos);
        }

        event->accept();
        return;
    }

    QGraphicsScene::mousePressEvent(event);
}

void GraphScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) {
    if(m_state == GraphScene::State::REMOVE || m_state == GraphScene::State::EDIT ||
       GraphScene::m_state == State::IDLE) {
        event->accept();
        return;
    }

    QGraphicsScene::mouseDoubleClickEvent(event);
}

void GraphScene::onNodeSelectTrigger(NodeItem* node) {
    if(m_state == GraphScene::State::ADD || m_state == GraphScene::State::EDIT) {
        selectNode(node);
    }

    if(m_state == GraphScene::State::REMOVE) {
        emit removeNodeRequest(node);
    }
}

void GraphScene::onEdgeSelectTrigger(EdgeItem* edge) {
    if(m_state == GraphScene::State::REMOVE) {
        emit removeEdgeRequest(edge);
    }
}

void GraphScene::setEditGraphSceneTrigger(bool edit, EditableTextItem* label) {
    if(edit) {
        m_state     = GraphScene::State::EDIT;
        m_editLabel = label;
    } else {
        m_state     = GraphScene::State::ADD;
        m_editLabel = nullptr;
    }
}

void GraphScene::addNode(Node* nodeModel) {
    if(nodeModel == nullptr) {
        return;
    }

    NodeItem* nodeItem = new NodeItem(nodeModel);
    addItem(nodeItem);
    m_nodeItems[nodeModel->getId()] = nodeItem;
    connect(nodeItem, &NodeItem::nodeSelected, this, &GraphScene::onNodeSelectTrigger);
    connect(nodeItem, &NodeItem::editNodeNameRequest, this, &GraphScene::editNodeNameRequest);
    connect(nodeItem->label(), &EditableTextItem::setEditGraphSceneState, this,
            &GraphScene::setEditGraphSceneTrigger);
    connect(nodeItem, &NodeItem::moveNodeRequest, this, &GraphScene::moveNodeRequest);
}

void GraphScene::addEdge(Edge* edgeModel, bool isDirected, bool isWeighted) {
    if(edgeModel == nullptr) {
        return;
    }

    NodeItem* src  = findNodeItemById(edgeModel->startNode());
    NodeItem* dest = findNodeItemById(edgeModel->endNode());

    if(edgeModel == nullptr || src == nullptr || dest == nullptr) {
        return;
    }

    EdgeItem* edgeItem = nullptr;

    if(isDirected) {
        edgeItem = new DirectedEdgeItem(edgeModel, src, dest, isWeighted);
    } else {
        edgeItem = new UndirectedEdgeItem(edgeModel, src, dest, isWeighted);
    }

    if(isWeighted) {
        edgeItem->initEdgeWeight();
        connect(edgeItem->weight(), &EditableTextItem::setEditGraphSceneState, this,
                &GraphScene::setEditGraphSceneTrigger);
    }

    edgeItem->adjust();
    addItem(edgeItem);
    edgeItem->updateSize();
    connect(edgeItem, &EdgeItem::edgeSelected, this, &GraphScene::onEdgeSelectTrigger);
    connect(edgeItem, &EdgeItem::editEdgeWeightRequest, this, &GraphScene::editEdgeWeightRequest);

    src->setNodeSelected(false);
    dest->setNodeSelected(false);
    m_firstNodeSelect = nullptr;
}

void GraphScene::addNodeAndEdge(Node* nodeModel, Edge* edgeModel, bool isDirected,
                                bool isWeighted) {
    addNode(nodeModel);
    addEdge(edgeModel, isDirected, isWeighted);
}

void GraphScene::removeNode(NodeItem* node) {
    if(node == nullptr) {
        return;
    }

    auto edges = node->edges();
    for(EdgeItem* edge: edges) {
        removeEdge(edge);
    }

    m_nodeItems.erase(node->modelNode()->getId());
    removeItem(node);
    delete node;
}

void GraphScene::removeEdge(EdgeItem* edge) {
    if(edge == nullptr) {
        return;
    }

    edge->disconnectNodes();
    removeItem(edge);
    delete edge;
}

EdgeItem* GraphScene::findEdgeItemById(unsigned edgeId) const {
    const QList<QGraphicsItem*> all = items();
    for(QGraphicsItem* it: all) {
        auto* edgeItem = dynamic_cast<EdgeItem*>(it);
        if(edgeItem == nullptr || edgeItem->modelEdge() == nullptr) {
            continue;
        }

        if(edgeItem->modelEdge()->getId() == edgeId) {
            return edgeItem;
        }
    }
    return nullptr;
}

NodeItem* GraphScene::findNodeItemById(const unsigned id) const {
    auto it = m_nodeItems.find(id);
    return (it != m_nodeItems.end()) ? it->second : nullptr;
}

EdgeItem* GraphScene::findEdgeItemByNodes(unsigned from, unsigned to) const {
    const QList<QGraphicsItem*> all = items();
    for(QGraphicsItem* it: all) {
        auto* edgeItem = dynamic_cast<EdgeItem*>(it);
        if(edgeItem == nullptr || edgeItem->modelEdge() == nullptr) {
            continue;
        }

        const unsigned a = edgeItem->modelEdge()->startNode();
        const unsigned b = edgeItem->modelEdge()->endNode();

        if((a == from && b == to) || (a == to && b == from)) {
            return edgeItem;
        }
    }
    return nullptr;
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
    emit addEdgeRequest(m_firstNodeSelect, node);
}

void GraphScene::updateNodeScalling() {
    for(auto* item: items()) {
        if(auto* n = dynamic_cast<NodeItem*>(item)) {
            n->updateSize();
        }

        if(auto* e = dynamic_cast<EdgeItem*>(item)) {
            e->updateSize();
        }
    }
    update();
}

void GraphScene::applyTheme(ThemeManager::Theme theme) {
    switch(theme) {
    case ThemeManager::Theme::LIGHT:
        setBackgroundBrush(QColor(245, 245, 245));
        break;
    case ThemeManager::Theme::DARK:
        setBackgroundBrush(QColor(60, 60, 60));
        break;
    case ThemeManager::Theme::PURPLE:
        setBackgroundBrush(QColor(90, 70, 120));
        break;
    }
    update();
}
