#include "GraphController.h"

GraphController::GraphController(QObject *parent) : QObject(parent){
}

GraphController::GraphController(GraphScene *scene, QObject *parent) : QObject(parent), m_scene(scene) {
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


