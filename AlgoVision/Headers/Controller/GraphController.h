#ifndef GRAPH_CONTROLLER_H
#define GRAPH_CONTROLLER_H

#include <QObject>
#include <memory.h>

#include "Graph.h"
#include "GraphScene.h"

class GraphController : public QObject {
public:
    explicit GraphController(QObject* parent = nullptr);
    explicit GraphController(GraphScene* scene, QObject* parent = nullptr);

    std::shared_ptr<Graph> graph() const;
    void setGraph(const std::shared_ptr<Graph> &newGraph);
    GraphScene *scene() const;
    void setScene(GraphScene *newScene);

public slots:
    std::shared_ptr<Graph> createGraph(bool isDirected, bool isWeighted);
    void addNode(const QPointF& point);
    void addEdge(NodeItem* source, NodeItem* dest);
    void removeNode(NodeItem* nodeItem);
    void removeEdge(EdgeItem* edgeItem);
    void clear();

private:
    std::shared_ptr<Graph> m_graph;
    GraphScene* m_scene;
};

#endif // GRAPH_CONTROLLER
