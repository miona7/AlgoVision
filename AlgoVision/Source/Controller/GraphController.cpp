#include "GraphController.h"
#include "WeightedDirectedGraph.h"
#include "WeightedUndirectedGraph.h"
#include "UnweightedDirectedGraph.h"
#include "UnweightedUndirectedGraph.h"

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
