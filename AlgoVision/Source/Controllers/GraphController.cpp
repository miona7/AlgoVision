#include "GraphController.h"
#include "EdgeItem.h"
#include "EditableTextItem.h"
#include "UnweightedDirectedGraph.h"
#include "UnweightedUndirectedGraph.h"
#include "WeightedDirectedGraph.h"
#include "WeightedUndirectedGraph.h"
#include <QUndoCommand>

struct EdgeSnapshot {
    unsigned from;
    unsigned to;
    int      weight;
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
        for(const auto& [_, e]: g->getEdges()) {
            if(e.startNode() == m_nodeId || e.endNode() == m_nodeId) {
                m_edges.push_back({e.startNode(), e.endNode(), e.getWeight()});
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
        //vrati cvor
        m_c->addNodeWithIdNoHistory(m_nodeId, QPointF(m_x, m_y));

        //vrati sve grane
        for(const EdgeSnapshot& es: m_edges) {
            m_c->addEdgeNoHistory(es.from, es.to, es.weight);
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
        m_c->addEdgeNoHistory(m_from, m_to, m_weight);
    }

    void undo() override {
        if(m_c == nullptr) {
            return;
        }
        m_c->removeEdgeNoHistory(m_from, m_to);
    }

private:
    GraphController* m_c;
    unsigned         m_from;
    unsigned         m_to;
    int              m_weight;
};

class RemoveEdgeCommand : public QUndoCommand {
public:
    RemoveEdgeCommand(GraphController* c, unsigned from, unsigned to, int weight)
        : m_c(c), m_from(from), m_to(to), m_weight(weight) {
        setText("Remove edge");
    }

    void redo() override {
        if(m_c == nullptr) {
            return;
        }
        m_c->removeEdgeNoHistory(m_from, m_to);
    }

    void undo() override {
        if(m_c == nullptr) {
            return;
        }
        m_c->addEdgeNoHistory(m_from, m_to, m_weight);
    }

private:
    GraphController* m_c;
    unsigned         m_from;
    unsigned         m_to;
    int              m_weight;
};

class EditEdgeWeightCommand : public QUndoCommand {
public:
    EditEdgeWeightCommand(GraphController* c, unsigned from, unsigned to, int beforeW,
                          QString beforeText, int afterW, QString afterText)
        : m_c(c), m_from(from), m_to(to), m_beforeW(beforeW), m_beforeText(std::move(beforeText)),
          m_afterW(afterW), m_afterText(std::move(afterText)) {
        setText("Edit edge weight");
    }

    void redo() override {
        if(m_c == nullptr) {
            return;
        }
        m_c->setEdgeWeightNoHistory(m_from, m_to, m_afterW, m_afterText);
    }

    void undo() override {
        if(m_c == nullptr) {
            return;
        }
        m_c->setEdgeWeightNoHistory(m_from, m_to, m_beforeW, m_beforeText);
    }

private:
    GraphController* m_c;
    unsigned         m_from, m_to;
    int              m_beforeW, m_afterW;
    QString          m_beforeText, m_afterText;
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
    if(m_undoStack) {
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
    if(!m_undoStack)
        return;

    m_undoStack->clear();
    clearNoHistory();
}

void GraphController::clearScene() const {
    m_scene->clear();
}

// trenutno netestiran metod, jer cuvanje i otvaranje grafa nije povezano
void GraphController::buildScene() const {
    // prvo dodajemo sve cvorove
    for(auto& [id, _]: m_graph->getNodes()) {
        Node* nodeModel = m_graph->getNode(id);
        m_scene->addNode(nodeModel);
    }

    // onda dodajemo sve grane
    for(auto& [id, _]: m_graph->getEdges()) {
        Edge* edgeModel = m_graph->getEdge(id);
        m_scene->addEdge(edgeModel, m_graph->isDirected(), m_graph->isWeighted());
    }
}

GraphScene* GraphController::scene() const {
    return m_scene.get();
}

void GraphController::updateNodePosition(NodeItem* nodeItem, const QPointF &point) {
    nodeItem->modelNode()->setPosition(point.x(), point.y());
    nodeItem->updateNodePosition();
}

// scena zahteva promene od kontrolera, ne vrsi ih direktno nad modelom, a ni nad sobom
void GraphController::connectScene() const {
    connect(m_scene.get(), &GraphScene::addNodeRequest, this, &GraphController::addNode);
    connect(m_scene.get(), &GraphScene::addEdgeRequest, this, &GraphController::addEdge);
    connect(m_scene.get(), &GraphScene::removeNodeRequest, this, &GraphController::removeNode);
    connect(m_scene.get(), &GraphScene::removeEdgeRequest, this, &GraphController::removeEdge);
    connect(m_scene.get(), &GraphScene::editNodeNameRequest, this, &GraphController::editNodeName);
    connect(m_scene.get(), &GraphScene::editEdgeWeightRequest, this,
            &GraphController::editEdgeWeight);
    connect(m_scene.get(), &GraphScene::moveNodeRequest, this, &GraphController::moveNode);
    connect(m_scene.get(), &GraphScene::addNodeAndEdgeRequest, this, &GraphController::addNodeAndEdge);
}

void GraphController::addNode(const QPointF& position) {
    if(m_graph == nullptr || m_undoStack == nullptr) {
        return;
    }
    m_undoStack->push(new AddNodeCommand(this, position));
}

void GraphController::addNodeAndEdge(const QPointF& point, NodeItem* selectedNodeItem) {
    if(selectedNodeItem == nullptr) {
        return;
    }

    // prvo dodajemo u model
    Node* nodeModel = m_graph->addNode(point.x(), point.y());
    unsigned srcId = selectedNodeItem->modelNode()->getId(); // fromID
    unsigned destId = nodeModel->getId(); // toID
    m_graph->addEdge(srcId, destId);

    // onda dodajemo u pogled
    Edge* edgeModel = m_graph->getEdge(srcId, destId);
    m_scene->addNodeAndEdge(nodeModel, edgeModel, m_graph->isDirected(), m_graph->isWeighted());
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

    // sprecavamo da dodamo vec postojecu granu, da dodamo istu granu vise puta
    if(m_graph->getEdge(sourceId, destId) != nullptr) {
        // mozda je stanje scene naruseno, cvor je selektovan i promenjena mu je boja, a operacija
        // je nevalidna
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
    if(m_graph == nullptr || m_undoStack == nullptr) {
        return;
    }
    if(edgeItem == nullptr || edgeItem->modelEdge() == nullptr) {
        return;
    }

    const unsigned from   = edgeItem->modelEdge()->startNode();
    const unsigned to     = edgeItem->modelEdge()->endNode();
    const int      weight = edgeItem->modelEdge()->getWeight();

    m_undoStack->push(new RemoveEdgeCommand(this, from, to, weight));
}

void GraphController::editNodeName(const NodeItem* nodeItem, const QString& name) {
    nodeItem->modelNode()->setName(name);

    emit sceneModified();
}

// ako korisnik unese nevalidnu tezinu grane, tezina grane se resetuje na prethodnu validnu
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
    const unsigned from = edgeItem->modelEdge()->startNode();
    const unsigned to   = edgeItem->modelEdge()->endNode();

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
        new EditEdgeWeightCommand(this, from, to, beforeW, beforeText, afterW, afterText));
}

void GraphController::moveNode(const NodeItem* nodeItem, const QPointF& oldPos, const QPointF& newPos) {
    //move command logic
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

void GraphController::clearNoHistory() {
    m_scene->clearSelection();
    m_scene->clearFocus();
    m_scene->setFocusItem(nullptr);
    m_scene->clear();
    if(m_graph) {
        m_graph->clear();
    }
    emit sceneModified();
}
