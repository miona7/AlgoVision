#include "GraphController.h"
#include "EdgeItem.h"
#include "EditableTextItem.h"
#include "UnweightedDirectedGraph.h"
#include "UnweightedUndirectedGraph.h"
#include "WeightedDirectedGraph.h"
#include "WeightedUndirectedGraph.h"

GraphController::GraphController(QObject* parent) : QObject(parent) {
    connectScene();
}

std::shared_ptr<Graph> GraphController::graph() const {
    return m_graph;
}

void GraphController::setGraph(const std::shared_ptr<Graph>& newGraph) {
    m_graph = newGraph;
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

void GraphController::clear() const {
    m_scene->clear(); // prvo obrisemo pogled
    m_graph->clear(); // onda obrisemo model
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

// scena zahteva promene od kontrolera, ne vrsi ih direktno nad modelom, a ni nad sobom
void GraphController::connectScene() const {
    connect(m_scene.get(), &GraphScene::addNodeRequest, this, &GraphController::addNode);
    connect(m_scene.get(), &GraphScene::addEdgeRequest, this, &GraphController::addEdge);
    connect(m_scene.get(), &GraphScene::removeNodeRequest, this, &GraphController::removeNode);
    connect(m_scene.get(), &GraphScene::removeEdgeRequest, this, &GraphController::removeEdge);
    connect(m_scene.get(), &GraphScene::editNodeNameRequest, this, &GraphController::editNodeName);
    connect(m_scene.get(), &GraphScene::editEdgeWeightRequest, this,
            &GraphController::editEdgeWeight);
}

void GraphController::addNode(const QPointF& position) {
    Node* nodeModel = m_graph->addNode(position.x(), position.y());
    m_scene->addNode(nodeModel);

    emit sceneModified();
}

void GraphController::addEdge(NodeItem* source, NodeItem* dest) {
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

    m_graph->addEdge(sourceId, destId);
    Edge* edgeModel = m_graph->getEdge(sourceId, destId);
    m_scene->addEdge(edgeModel, m_graph->isDirected(), m_graph->isWeighted());

    emit sceneModified();
}

void GraphController::removeNode(NodeItem* nodeItem) {
    if(nodeItem == nullptr) {
        return;
    }

    const unsigned nodeId = nodeItem->modelNode()->getId();
    m_scene->removeNode(nodeItem); // prvo brisemo pogled
    m_graph->removeNode(nodeId);   // pa onda brisemo model

    emit sceneModified();
}

void GraphController::removeEdge(EdgeItem* edgeItem) {
    if(edgeItem == nullptr) {
        return;
    }

    const unsigned edgeId = edgeItem->modelEdge()->getId();
    m_scene->removeEdge(edgeItem); // prvo brisemo pogled
    m_graph->removeEdge(edgeId);   // pa onda brisemo model

    emit sceneModified();
}

void GraphController::editNodeName(const NodeItem* nodeItem, const QString& name) {
    nodeItem->modelNode()->setName(name);

    emit sceneModified();
}

// ako korisnik unese nevalidnu tezinu grane, tezina grane se resetuje na prethodnu validnu
void GraphController::editEdgeWeight(const EdgeItem* edgeItem, const QString& weight) {
    bool isNumber;
    int  number = weight.toInt(&isNumber);
    if(isNumber) {
        edgeItem->modelEdge()->setWeight(number);
        edgeItem->adjustWeightGeometry();

        emit sceneModified();
    } else {
        EditableTextItem* weightLabel = edgeItem->weight();
        weightLabel->setPlainText(weightLabel->oldText());
    }
}
