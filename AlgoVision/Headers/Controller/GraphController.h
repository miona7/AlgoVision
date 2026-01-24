#ifndef GRAPH_CONTROLLER_H
#define GRAPH_CONTROLLER_H

#include <QObject>
#include <memory.h>

#include "Graph.h"
#include "GraphScene.h"

class GraphController : public QObject {
    Q_OBJECT

public:
    explicit GraphController(QObject* parent = nullptr);
    // explicit GraphController(GraphScene* scene, QObject* parent = nullptr);

    std::shared_ptr<Graph> graph() const;
    void setGraph(const std::shared_ptr<Graph> &newGraph);

    void setAddSceneState() const;
    void setRemoveSceneState() const;

    GraphScene* scene() const;

public slots:
    void createGraph(bool isDirected, bool isWeighted);
    void addNode(const QPointF& point);
    void addEdge(NodeItem* source, NodeItem* dest);
    void removeNode(NodeItem* nodeItem);
    void removeEdge(EdgeItem* edgeItem);
    void clear();

private:
    std::shared_ptr<Graph> m_graph;
    // scena treba da se nalazi samo u okviru kontrolera, i on je vlasnik scene
    std::unique_ptr<GraphScene> m_scene = std::make_unique<GraphScene>();

    void connectScene() const;
};

#endif // GRAPH_CONTROLLER
