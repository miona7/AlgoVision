#ifndef GRAPH_CONTROLLER_H
#define GRAPH_CONTROLLER_H

#include <QObject>
#include <memory.h>

#include "Graph.h"
#include "GraphScene.h"

class GraphController : public QObject {
    Q_OBJECT

public:
    explicit GraphController(QObject* = nullptr);

    std::shared_ptr<Graph> graph() const;
    void                   setGraph(const std::shared_ptr<Graph>&);

    void setAddSceneState() const;
    void setRemoveSceneState() const;

    GraphScene* scene() const;

public slots:
    void createGraph(bool, bool);
    void addNode(const QPointF&) const;
    void addEdge(NodeItem*, NodeItem*) const;
    void removeNode(NodeItem*) const;
    void removeEdge(EdgeItem*) const;
    void editNodeName(const NodeItem*, const QString&) const;
    void editEdgeWeight(const EdgeItem*, const QString&) const;

    // ova metoda brise sadrzaj modela grafa(grane i cvorove) i
    // pogleda grafa(cvor/grana ajteme), ali se nikad ne brisu m_graph i m_scene
    // oni postoje dok postoji i kontroler
    void clear() const;
    void clearScene() const;

    // pravi scenu od vec ucitanog grafa
    void buildScene() const;

    // kontroler je vlasnik i upravlja nad modelom i pogledom grafa
private:
    std::shared_ptr<Graph>      m_graph;
    std::unique_ptr<GraphScene> m_scene = std::make_unique<GraphScene>();

    void connectScene() const;
};

#endif // GRAPH_CONTROLLER_H
