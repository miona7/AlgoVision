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
    void setGraph(const std::shared_ptr<Graph>&);

    void setAddSceneState() const;
    void setRemoveSceneState() const;

    GraphScene* scene() const;

public slots:
    void createGraph(bool, bool);
    void addNode(const QPointF&);
    void addEdge(NodeItem*, NodeItem*);
    void removeNode(NodeItem*);
    void removeEdge(EdgeItem*);

    // ova metoda brise sadrzaj modela grafa(grane i cvorove) i
    // pogleda grafa(cvor/grana ajteme), ali se nikad ne brisu m_graph i m_scene
    // oni postoje dok postoji i kontroler
    void clear();

// kontroler je vlasnik i upravlja nad modelom i pogledom grafa
private:
    std::shared_ptr<Graph> m_graph;
    std::unique_ptr<GraphScene> m_scene = std::make_unique<GraphScene>();

    void connectScene() const;
};

#endif // GRAPH_CONTROLLER
