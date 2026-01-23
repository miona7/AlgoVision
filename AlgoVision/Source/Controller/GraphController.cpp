#include "GraphController.h"
#include "EdgeItem.h"
#include "WeightedDirectedGraph.h"
#include "WeightedUndirectedGraph.h"
#include "UnweightedDirectedGraph.h"
#include "UnweightedUndirectedGraph.h"

GraphController::GraphController(QObject *parent) : QObject(parent){
    connectScene();
}

GraphController::GraphController(GraphScene *scene, QObject *parent) : QObject(parent), m_scene(scene) {
    connectScene();
}

std::shared_ptr<Graph> GraphController::graph() const {
    return m_graph;
}

void GraphController::setGraph(const std::shared_ptr<Graph>& newGraph) {
    m_graph = newGraph;
}

GraphScene *GraphController::scene() const {
    return m_scene;
}

void GraphController::setScene(GraphScene* newScene) {
    m_scene = newScene;
}

std::shared_ptr<Graph> GraphController::createGraph(bool isDirected, bool isWeighted) {
    if (isDirected && isWeighted) {
        return std::make_shared<WeightedDirectedGraph>();
    }

    if (isDirected && !isWeighted) {
        return std::make_shared<UnweightedDirectedGraph>();
    }

    if (!isDirected && isWeighted) {
        return std::make_shared<WeightedUndirectedGraph>();
    }

    return std::make_shared<UnweightedUndirectedGraph>();
}

void GraphController::clear() {
    m_scene->clear(); // prvo obrisemo pogled
    m_graph->clear(); // onda obrisemo model
}

void GraphController::connectScene() const {
    connect(m_scene, &GraphScene::addNodeRequest, this, &GraphController::addNode);
    connect(m_scene, &GraphScene::addEdgeRequest, this, &GraphController::addEdge);
    connect(m_scene, &GraphScene::removeNodeRequest, this, &GraphController::removeNode);
    connect(m_scene, &GraphScene::removeEdgeRequest, this, &GraphController::removeEdge);
}

void GraphController::addNode(const QPointF &position) {
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
    m_graph->removeNode(nodeId); // pa onda brisemo model
}

void GraphController::removeEdge(EdgeItem* edgeItem) {
    const unsigned edgeId = edgeItem->modelEdge()->getId();
    m_scene->removeEdge(edgeItem); // prvo brisemo pogled
    m_graph->removeEdge(edgeId); // pa onda brisemo model
}
