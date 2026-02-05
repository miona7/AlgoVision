#include "GraphController.h"
#include "EdgeItem.h"
#include "EditableTextItem.h"
#include "UnweightedDirectedGraph.h"
#include "UnweightedUndirectedGraph.h"
#include "WeightedDirectedGraph.h"
#include "WeightedUndirectedGraph.h"
#include <QUndoCommand>

struct EdgeSnapshot {
    unsigned m_id;
    unsigned m_from;
    unsigned m_to;
    int      m_weight;
};

struct NodeSnapshot {
    unsigned m_id;
    double   m_x;
    double   m_y;
};

class AddNodeCommand : public QUndoCommand {
public:
    AddNodeCommand(GraphController* c, const QPointF& pos) : m_c(c), m_pos(pos) {
    }

    void redo() override {
        if(m_c == nullptr) {
            return;
        }
        if(!m_hasId) {
            m_c->addNodeNoHistory(m_pos, m_id);
            m_hasId = true;
        } else {
            m_c->addNodeWithIdNoHistory(m_id, m_pos);
        }
    }

    void undo() override {
        if(m_c == nullptr) {
            return;
        }
        m_c->removeNodeNoHistory(m_id);
    }

private:
    GraphController* m_c;
    QPointF          m_pos;
    unsigned         m_id    = 0;
    bool             m_hasId = false;
};

class RemoveNodeCommand : public QUndoCommand {
public:
    RemoveNodeCommand(GraphController* c, unsigned nodeId) : m_c(c), m_nodeId(nodeId) {
        setText("Remove node");
    }

    // pozovi PRE push-a
    void capture() {
        if(m_c == nullptr) {
            return;
        }

        auto g = m_c->graph();
        if(g == nullptr) {
            return;
        }

        const Node* n = g->getNode(m_nodeId);
        if(n == nullptr) {
            return;
        }
        // zapamti poziciju
        if(NodeItem* ni = m_c->scene()->findNodeItemById(m_nodeId)) {
            QPointF p = ni->pos();
            m_x       = p.x();
            m_y       = p.y();
        }

        // zapamti sve incident grane
        for(const auto& [id, e]: g->getEdges()) {
            if(e.startNode() == m_nodeId || e.endNode() == m_nodeId) {
                m_edges.push_back({e.getId(), e.startNode(), e.endNode(), e.getWeight()});
            }
        }
    }

    void redo() override {
        if(m_c == nullptr) {
            return;
        }
        m_c->removeNodeNoHistory(m_nodeId);
    }

    void undo() override {
        if(m_c == nullptr) {
            return;
        }
        // vrati cvor
        m_c->addNodeWithIdNoHistory(m_nodeId, QPointF(m_x, m_y));

        // vrati sve grane
        for(const EdgeSnapshot& es: m_edges) {
            m_c->restoreEdgeNoHistory(es.m_id, es.m_from, es.m_to, es.m_weight);
        }
    }

private:
    GraphController* m_c;
    unsigned         m_nodeId;

    double m_x = 0;
    double m_y = 0;

    std::vector<EdgeSnapshot> m_edges;
};

class AddEdgeCommand : public QUndoCommand {
public:
    AddEdgeCommand(GraphController* c, unsigned from, unsigned to, int weight)
        : m_c(c), m_from(from), m_to(to), m_weight(weight) {
        setText("Add edge");
    }

    void redo() override {
        if(m_c == nullptr) {
            return;
        }
        if(!m_hasId) {
            m_c->addEdgeNoHistory(m_from, m_to, m_weight);
            if(Edge* e = m_c->graph()->getEdge(m_from, m_to)) {
                m_edgeId = e->getId();
                m_hasId  = true;
            }
        } else {
            m_c->restoreEdgeNoHistory(m_edgeId, m_from, m_to, m_weight);
        }
    }

    void undo() override {
        if(m_c == nullptr || !m_hasId) {
            return;
        }
        m_c->removeEdgeNoHistoryById(m_edgeId);
    }

private:
    GraphController* m_c;
    unsigned         m_from;
    unsigned         m_to;
    int              m_weight;
    unsigned         m_edgeId = 0;
    bool             m_hasId  = false;
};

class RemoveEdgeCommand : public QUndoCommand {
public:
    RemoveEdgeCommand(GraphController* c, EdgeItem* edgeItem) : m_c(c) {

        Edge* e  = edgeItem->modelEdge();
        m_edgeId = e->getId();
        m_from   = e->startNode();
        m_to     = e->endNode();
        m_weight = e->getWeight();
    }

    void redo() override {
        if(m_c != nullptr) {
            m_c->removeEdgeNoHistoryById(m_edgeId);
        }
    }

    void undo() override {
        if(m_c != nullptr) {
            m_c->restoreEdgeNoHistory(m_edgeId, m_from, m_to, m_weight);
        }
    }

private:
    GraphController* m_c      = nullptr;
    unsigned         m_edgeId = 0;
    unsigned         m_from   = 0;
    unsigned         m_to     = 0;
    int              m_weight = 1;
};

class EditEdgeWeightCommand : public QUndoCommand {
public:
    EditEdgeWeightCommand(GraphController* c, unsigned edgeId, int beforeW, QString beforeText,
                          int afterW, QString afterText)
        : m_c(c), m_edgeId(edgeId), m_beforeW(beforeW), m_afterW(afterW),
          m_beforeText(std::move(beforeText)), m_afterText(std::move(afterText)) {
        setText("Edit edge weight");
    }

    void redo() override {
        if(m_c == nullptr) {
            return;
        }
        m_c->setEdgeWeightNoHistoryById(m_edgeId, m_afterW, m_afterText);
    }

    void undo() override {
        if(m_c == nullptr) {
            return;
        }
        m_c->setEdgeWeightNoHistoryById(m_edgeId, m_beforeW, m_beforeText);
    }

private:
    GraphController* m_c = nullptr;

    unsigned m_edgeId = 0;

    int m_beforeW = 0;
    int m_afterW  = 0;

    QString m_beforeText;
    QString m_afterText;
};

class EditNodeNameCommand : public QUndoCommand {
public:
    EditNodeNameCommand(GraphController* c, unsigned nodeId, QString beforeName, QString afterName)
        : m_c(c), m_nodeId(nodeId), m_beforeName(std::move(beforeName)),
          m_afterName(std::move(afterName)) {
        setText("Edit node name");
    }

    void redo() override {
        if(m_c == nullptr) {
            return;
        }
        m_c->setNodeNameNoHistory(m_nodeId, m_afterName);
    }

    void undo() override {
        if(m_c == nullptr) {
            return;
        }
        m_c->setNodeNameNoHistory(m_nodeId, m_beforeName);
    }

private:
    GraphController* m_c;
    unsigned         m_nodeId;
    QString          m_beforeName;
    QString          m_afterName;
};

class AddNodeAndEdgeCommand : public QUndoCommand {
public:
    AddNodeAndEdgeCommand(GraphController* c, unsigned fromId, const QPointF& pos)
        : m_c(c), m_fromId(fromId), m_pos(pos) {
        setText("Add node and edge");
    }

    void redo() override {
        if(m_c == nullptr) {
            return;
        }

        if(!m_hasId) {
            m_c->addNodeNoHistory(m_pos, m_newId);
            m_hasId = true;
        } else {
            m_c->addNodeWithIdNoHistory(m_newId, m_pos);
        }

        m_c->addEdgeNoHistory(m_fromId, m_newId, 1);
    }

    void undo() override {
        if(m_c == nullptr) {
            return;
        }

        m_c->removeEdgeNoHistory(m_fromId, m_newId);
        m_c->removeNodeNoHistory(m_newId);
    }

private:
    GraphController* m_c;
    unsigned         m_fromId;
    QPointF          m_pos;

    unsigned m_newId = 0;
    bool     m_hasId = false;
};

class MoveNodeCommand : public QUndoCommand {
public:
    MoveNodeCommand(GraphController* c, unsigned nodeId, const QPointF& oldPos,
                    const QPointF& newPos)
        : m_c(c), m_nodeId(nodeId), m_oldPos(oldPos), m_newPos(newPos) {
        setText("Move node");
    }

    void redo() override {
        if(m_c == nullptr) {
            return;
        }
        m_c->moveNodeNoHistory(m_nodeId, m_newPos);
    }

    void undo() override {
        if(m_c == nullptr) {
            return;
        }
        m_c->moveNodeNoHistory(m_nodeId, m_oldPos);
    }

private:
    GraphController* m_c      = nullptr;
    unsigned         m_nodeId = 0;
    QPointF          m_oldPos;
    QPointF          m_newPos;
};


class ClearGraphCommand : public QUndoCommand {
public:
    ClearGraphCommand(GraphController* c) : m_c(c) {
        setText("Clear graph");
    }

    void capture() {
        if(m_c == nullptr) {
            return;
        }

        auto g = m_c->graph();
        if(g == nullptr) {
            return;
        }

        for(const auto& [id, _] : g->getNodes()) {
            if(NodeItem* ni = m_c->scene()->findNodeItemById(id)) {
                QPointF p = ni->pos();
                m_nodes.push_back({id, p.x(), p.y()});
            }
        }

        for(const auto& [id, _] : g->getEdges()) {
            if(Edge* e = g->getEdge(id)) {
                m_edges.push_back(
                    {e->getId(), e->startNode(), e->endNode(), e->getWeight()});
            }
        }
    }

    void redo() override {
        if(m_c == nullptr) {
            return;
        }
        m_c->clearNoHistory();
    }

    void undo() override {
        if(m_c == nullptr) {
            return;
        }

        for(const NodeSnapshot& n : m_nodes) {
            m_c->addNodeWithIdNoHistory(n.m_id, QPointF(n.m_x, n.m_y));
        }

        for(const EdgeSnapshot& e : m_edges) {
            m_c->restoreEdgeNoHistory(e.m_id, e.m_from, e.m_to, e.m_weight);
        }
    }

private:
    GraphController* m_c;

    std::vector<NodeSnapshot> m_nodes;
    std::vector<EdgeSnapshot> m_edges;
};

GraphController::GraphController(QObject* parent) : QObject(parent) {
    m_undoStack = new QUndoStack(this);
    connectScene();
}

QUndoStack* GraphController::undoStack() const {
    return m_undoStack;
}

std::shared_ptr<Graph> GraphController::graph() const {
    return m_graph;
}

void GraphController::setGraph(const std::shared_ptr<Graph>& newGraph) {
    if(m_undoStack != nullptr) {
        m_undoStack->clear();
    }
    m_graph = newGraph;
    clearNoHistory(); // ciscenje istorije bez dodavanja komande u undo
}

void GraphController::setAddSceneState() const {
    m_scene->resetScene();
    m_scene->setState(GraphScene::State::ADD);
}

void GraphController::setRemoveSceneState() const {
    m_scene->resetScene();
    m_scene->setState(GraphScene::State::REMOVE);
}

void GraphController::createGraph(bool isDirected, bool isWeighted) {
    if(isDirected && isWeighted) {
        m_graph = std::make_shared<WeightedDirectedGraph>();
        return;
    }

    if(isDirected && !isWeighted) {
        m_graph = std::make_shared<UnweightedDirectedGraph>();
        return;
    }

    if(!isDirected && isWeighted) {
        m_graph = std::make_shared<WeightedUndirectedGraph>();
        return;
    }

    m_graph = std::make_shared<UnweightedUndirectedGraph>();
}

void GraphController::clear() {
    if(m_graph == nullptr || m_undoStack == nullptr) {
        return;
    }

    if(m_graph->getNodes().empty()) {
        return;
    }

    auto* cmd = new ClearGraphCommand(this);
    cmd->capture();
    m_undoStack->push(cmd);
}

void GraphController::clearScene() const {
    m_scene->clear();
}

void GraphController::buildScene() const {
    for(auto& [id, _]: m_graph->getNodes()) {
        Node* nodeModel = m_graph->getNode(id);
        m_scene->addNode(nodeModel);
    }

    for(auto& [id, _]: m_graph->getEdges()) {
        Edge* edgeModel = m_graph->getEdge(id);
        m_scene->addEdge(edgeModel, m_graph->isDirected(), m_graph->isWeighted());
    }
}

void GraphController::disableScene() const {
    m_scene->disableScene();
}

void GraphController::enableScene() const {
    m_scene->enableScene();
}

void GraphController::resetScene() const {
    m_scene->resetScene();
}

GraphScene* GraphController::scene() const {
    return m_scene.get();
}

void GraphController::updateNodePosition(NodeItem* nodeItem, const QPointF& point) {
    nodeItem->modelNode()->setPosition(point.x(), point.y());
    nodeItem->updateNodePosition();
}

void GraphController::connectScene() const {
    connect(m_scene.get(), &GraphScene::addNodeRequest, this, &GraphController::addNode);
    connect(m_scene.get(), &GraphScene::addEdgeRequest, this, &GraphController::addEdge);
    connect(m_scene.get(), &GraphScene::removeNodeRequest, this, &GraphController::removeNode);
    connect(m_scene.get(), &GraphScene::removeEdgeRequest, this, &GraphController::removeEdge);
    connect(m_scene.get(), &GraphScene::editNodeNameRequest, this, &GraphController::editNodeName);
    connect(m_scene.get(), &GraphScene::editEdgeWeightRequest, this,
            &GraphController::editEdgeWeight);
    connect(m_scene.get(), &GraphScene::moveNodeRequest, this, &GraphController::moveNode);
    connect(m_scene.get(), &GraphScene::addNodeAndEdgeRequest, this,
            &GraphController::addNodeAndEdge);
}

void GraphController::addNode(const QPointF& position) {
    if(m_graph == nullptr || m_undoStack == nullptr) {
        return;
    }
    m_undoStack->push(new AddNodeCommand(this, position));
}

void GraphController::addNodeAndEdge(const QPointF& point, NodeItem* selectedNodeItem) {
    if(m_graph == nullptr || m_undoStack == nullptr) {
        return;
    }
    if(selectedNodeItem == nullptr || selectedNodeItem->modelNode() == nullptr) {
        return;
    }

    const unsigned fromId = selectedNodeItem->modelNode()->getId();

    m_undoStack->push(new AddNodeAndEdgeCommand(this, fromId, point));
}

void GraphController::addEdge(NodeItem* source, NodeItem* dest) {
    if(m_graph == nullptr || m_undoStack == nullptr) {
        return;
    }
    if(source == nullptr || dest == nullptr) {
        return;
    }

    unsigned sourceId = source->modelNode()->getId();
    unsigned destId   = dest->modelNode()->getId();

    // disallow adding alredy existing edge (revert operation)
    if(m_graph->getEdge(sourceId, destId) != nullptr) {
        source->setNodeSelected(false);
        dest->setNodeSelected(false);
        m_scene->resetScene();
        return;
    }

    const int weight = 1;
    m_undoStack->push(new AddEdgeCommand(this, sourceId, destId, weight));
}

void GraphController::removeNode(NodeItem* nodeItem) {
    if(m_graph == nullptr || m_undoStack == nullptr) {
        return;
    }
    if(nodeItem == nullptr || nodeItem->modelNode() == nullptr) {
        return;
    }

    const unsigned nodeId = nodeItem->modelNode()->getId();

    auto* cmd = new RemoveNodeCommand(this, nodeId);
    cmd->capture();         // snimi stanje PRE brisanja
    m_undoStack->push(cmd); // Qt ce pozvati redo()
}

void GraphController::removeEdge(EdgeItem* edgeItem) {
    if(m_graph == nullptr || m_undoStack == nullptr || edgeItem == nullptr ||
       edgeItem->modelEdge() == nullptr) {
        return;
    }

    m_undoStack->push(new RemoveEdgeCommand(this, edgeItem));
}

void GraphController::editNodeName(const NodeItem* nodeItem, const QString& name) {
    if(m_graph == nullptr || m_undoStack == nullptr) {
        return;
    }
    if(nodeItem == nullptr || nodeItem->modelNode() == nullptr) {
        return;
    }

    const unsigned nodeId = nodeItem->modelNode()->getId();

    Node* n = m_graph->getNode(nodeId);
    if(n == nullptr) {
        return;
    }

    const QString beforeName = n->getName();
    const QString afterName  = name;

    if(beforeName == afterName) {
        return;
    }

    m_undoStack->push(new EditNodeNameCommand(this, nodeId, beforeName, afterName));
}

// if user enters invalid edge weight, it resets to previous one
void GraphController::editEdgeWeight(const EdgeItem* edgeItem, const QString& weight) {
    if(m_graph == nullptr || m_undoStack == nullptr) {
        return;
    }
    if(edgeItem == nullptr || edgeItem->modelEdge() == nullptr) {
        return;
    }
    bool ok   = false;
    int  newW = weight.toInt(&ok);
    if(!ok) {
        if(auto* w = edgeItem->weight()) {
            w->setPlainText(w->oldText());
            w->centerText();
        }
        return;
    }
    const unsigned from   = edgeItem->modelEdge()->startNode();
    const unsigned to     = edgeItem->modelEdge()->endNode();
    const unsigned edgeId = edgeItem->modelEdge()->getId();

    Edge* e = m_graph->getEdge(from, to);
    if(e == nullptr) {
        return;
    }

    const int     beforeW = e->getWeight();
    const QString beforeText =
        edgeItem->weight() ? edgeItem->weight()->oldText() : QString::number(beforeW);

    const int     afterW    = newW;
    const QString afterText = weight;

    if(beforeW == afterW && beforeText == afterText) {
        return;
    }

    m_undoStack->push(
        new EditEdgeWeightCommand(this, edgeId, beforeW, beforeText, afterW, afterText));
}

void GraphController::moveNode(const NodeItem* nodeItem, const QPointF& oldPos,
                               const QPointF& newPos) {
    if(m_graph == nullptr || m_undoStack == nullptr || nodeItem == nullptr ||
       nodeItem->modelNode() == nullptr) {
        return;
    }

    if(oldPos == newPos) {
        return;
    }

    const unsigned nodeId = nodeItem->modelNode()->getId();

    m_undoStack->push(new MoveNodeCommand(this, nodeId, oldPos, newPos));
}

void GraphController::addNodeNoHistory(const QPointF& pos, unsigned& outId) {
    if(m_graph == nullptr) {
        return;
    }
    Node* n = m_graph->addNode(pos.x(), pos.y());
    if(n == nullptr) {
        return;
    }
    outId = n->getId();
    m_scene->addNode(n);

    emit sceneModified();
}

void GraphController::addNodeWithIdNoHistory(unsigned id, const QPointF& pos) {
    if(m_graph == nullptr) {
        return;
    }
    m_graph->addNode(id, pos.x(), pos.y());

    Node* n = m_graph->getNode(id);
    if(n == nullptr) {
        return;
    }
    m_scene->addNode(n);

    emit sceneModified();
}

void GraphController::removeNodeNoHistory(unsigned id) {
    if(m_graph == nullptr) {
        return;
    }

    m_scene->clearSelection();
    m_scene->clearFocus();
    m_scene->setFocusItem(nullptr);

    if(NodeItem* ni = m_scene->findNodeItemById(id)) {
        m_scene->removeNode(ni);
    }

    m_graph->removeNode(id);

    emit sceneModified();
}

void GraphController::addEdgeNoHistory(unsigned from, unsigned to, int weight) {
    if(m_graph == nullptr) {
        return;
    }
    if(from == to) {
        return;
    }
    if(m_graph->getEdge(from, to) != nullptr) {
        return;
    }

    if(m_graph->isWeighted()) {
        m_graph->addEdge(from, to, weight);
    } else {
        m_graph->addEdge(from, to);
    }

    Edge* e = m_graph->getEdge(from, to);
    if(e == nullptr) {
        return;
    }

    m_scene->addEdge(e, m_graph->isDirected(), m_graph->isWeighted());

    emit sceneModified();
}

void GraphController::removeEdgeNoHistory(unsigned from, unsigned to) {
    if(m_graph == nullptr) {
        return;
    }

    m_scene->clearSelection();
    m_scene->clearFocus();
    m_scene->setFocusItem(nullptr);

    if(EdgeItem* ei = m_scene->findEdgeItemByNodes(from, to)) {
        m_scene->removeEdge(ei);
    }

    if(Edge* e = m_graph->getEdge(from, to)) {
        m_graph->removeEdge(e->getId());
    }

    emit sceneModified();
}

void GraphController::setEdgeWeightNoHistory(unsigned from, unsigned to, int weight,
                                             const QString& weightText) {
    if(m_graph == nullptr) {
        return;
    }

    if(Edge* e = m_graph->getEdge(from, to)) {
        e->setWeight(weight);
    }

    if(EdgeItem* ei = m_scene->findEdgeItemByNodes(from, to)) {
        if(auto* w = ei->weight()) {
            w->setPlainText(weightText);
            w->setOldText(weightText);
            w->centerText();
        }
        ei->adjustWeightGeometry();
    }

    emit sceneModified();
}

void GraphController::setNodeNameNoHistory(unsigned nodeId, const QString& name) {
    if(m_graph == nullptr) {
        return;
    }

    if(Node* n = m_graph->getNode(nodeId)) {
        n->setName(name);
    }

    if(NodeItem* ni = m_scene->findNodeItemById(nodeId)) {
        if(auto* lbl = ni->label()) {
            lbl->setPlainText(name);
            lbl->setOldText(name);
            lbl->centerText();
        }
    }

    emit sceneModified();
}

void GraphController::removeEdgeNoHistoryById(unsigned edgeId) {
    if(m_graph == nullptr) {
        return;
    }

    m_scene->clearSelection();
    m_scene->clearFocus();
    m_scene->setFocusItem(nullptr);

    if(EdgeItem* ei = m_scene->findEdgeItemById(edgeId)) {
        m_scene->removeEdge(ei);
    }

    m_graph->removeEdge(edgeId);
    emit sceneModified();
}

void GraphController::addEdgeNoHistoryById(unsigned edgeId) {
    if(m_graph == nullptr) {
        return;
    }
    Edge* e = m_graph->getEdge(edgeId);
    if(e == nullptr) {
        return;
    }
    m_scene->addEdge(e, m_graph->isDirected(), m_graph->isWeighted());
    emit sceneModified();
}

void GraphController::restoreEdgeNoHistory(unsigned edgeId, unsigned from, unsigned to,
                                           int weight) {
    if(m_graph == nullptr) {
        return;
    }

    m_graph->addEdgeSerialized(edgeId, from, to, weight);

    if(Edge* e = m_graph->getEdge(edgeId)) {
        m_scene->addEdge(e, m_graph->isDirected(), m_graph->isWeighted());
    }

    emit sceneModified();
}

void GraphController::setEdgeWeightNoHistoryById(unsigned edgeId, int weight, const QString& text) {
    if(m_graph == nullptr) {
        return;
    }

    if(Edge* e = m_graph->getEdge(edgeId)) {
        e->setWeight(weight);
    }

    if(EdgeItem* ei = m_scene->findEdgeItemById(edgeId)) {
        if(auto* w = ei->weight()) {
            w->setPlainText(text);
            w->setOldText(text);
            w->centerText();
        }
        ei->adjustWeightGeometry();
    }

    emit sceneModified();
}

void GraphController::moveNodeNoHistory(unsigned nodeId, const QPointF& pos) {
    if(m_graph == nullptr) {
        return;
    }
    if(Node* n = m_graph->getNode(nodeId)) {
        n->setPosition(pos.x(), pos.y());
    }

    if(NodeItem* ni = m_scene->findNodeItemById(nodeId)) {
        ni->setPos(pos);
        ni->updateNodePosition();
    }

    emit sceneModified();
}

void GraphController::clearNoHistory() {
    m_scene->clearSelection();
    m_scene->clearFocus();
    m_scene->setFocusItem(nullptr);
    m_scene->clear();
    if(m_graph != nullptr) {
        m_graph->clear();
    }
    emit sceneModified();
}
