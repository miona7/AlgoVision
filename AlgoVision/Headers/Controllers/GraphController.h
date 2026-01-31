#ifndef GRAPH_CONTROLLER_H
#define GRAPH_CONTROLLER_H

#include <QObject>
#include <memory>

#include "Graph.h"
#include "GraphScene.h"
#include <QUndoStack>

class GraphController : public QObject {
    Q_OBJECT

public:
    explicit GraphController(QObject* = nullptr);

    std::shared_ptr<Graph> graph() const;
    void                   setGraph(const std::shared_ptr<Graph>&);

    void setAddSceneState() const;
    void setRemoveSceneState() const;

    QUndoStack* undoStack() const;

    GraphScene* scene() const;

signals:

    void sceneModified();

    // obrisani const zbog emitovanja signala za promenu grafa i cuvanja u fajl
public slots:
    void createGraph(bool, bool);
    void addNode(const QPointF&);
    void addEdge(NodeItem*, NodeItem*);
    void removeNode(NodeItem*);
    void removeEdge(EdgeItem*);
    void editNodeName(const NodeItem*, const QString&);
    void editEdgeWeight(const EdgeItem*, const QString&);

    // ova metoda brise sadrzaj modela grafa(grane i cvorove) i
    // pogleda grafa(cvor/grana ajteme), ali se nikad ne brisu m_graph i m_scene
    // oni postoje dok postoji i kontroler
    void clear();
    void clearScene() const;

    // pravi scenu od vec ucitanog grafa
    void buildScene() const;

    void addNodeNoHistory(const QPointF&, unsigned&);
    void addNodeWithIdNoHistory(unsigned, const QPointF&);
    void removeNodeNoHistory(unsigned);
    void addEdgeNoHistory(unsigned, unsigned, int);
    void removeEdgeNoHistory(unsigned, unsigned);
    void setEdgeWeightNoHistory(unsigned, unsigned, int, const QString&);
    void clearNoHistory();

    // kontroler je vlasnik i upravlja nad modelom i pogledom grafa
private:
    std::shared_ptr<Graph>      m_graph;
    std::unique_ptr<GraphScene> m_scene     = std::make_unique<GraphScene>();
    QUndoStack*                 m_undoStack = nullptr;

    void connectScene() const;
};

#endif // GRAPH_CONTROLLER_H
