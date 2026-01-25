#include "GraphController.h"
#include "EdgeItem.h"
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
    m_scene->setState(GraphScene::State::ADD);
}

void GraphController::setRemoveSceneState() const {
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
    m_scene->clear(); // prvo obrisemo pogled
    m_graph->clear(); // onda obrisemo model
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
}

void GraphController::addNode(const QPointF& position) {
    Node* nodeModel = m_graph->addNode(position.x(), position.y());
    m_scene->addNode(nodeModel);
}

void GraphController::addEdge(NodeItem* source, NodeItem* dest) {
    unsigned sourceId = source->modelNode()->getId();
    unsigned destId   = dest->modelNode()->getId();
    m_graph->addEdge(sourceId, destId);
    Edge* edgeModel = m_graph->getEdge(sourceId, destId);
    m_scene->addEdge(edgeModel, source, dest, m_graph->isDirected(), m_graph->isWeighted());
}

void GraphController::removeNode(NodeItem* nodeItem) {
    const unsigned nodeId = nodeItem->modelNode()->getId();
    m_scene->removeNode(nodeItem); // prvo brisemo pogled
    m_graph->removeNode(nodeId);   // pa onda brisemo model
}

void GraphController::removeEdge(EdgeItem* edgeItem) {
    const unsigned edgeId = edgeItem->modelEdge()->getId();
    m_scene->removeEdge(edgeItem); // prvo brisemo pogled
    m_graph->removeEdge(edgeId);   // pa onda brisemo model
}
