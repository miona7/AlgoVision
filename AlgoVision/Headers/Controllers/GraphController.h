#ifndef GRAPH_CONTROLLER_H
#define GRAPH_CONTROLLER_H

#include <QObject>
#include <QUndoStack>
#include <QUndoCommand>

#include <memory>

#include "Graph.h"
#include "GraphScene.h"
#include "EdgeItem.h"
#include "EditableTextItem.h"
#include "UnweightedDirectedGraph.h"
#include "UnweightedUndirectedGraph.h"
#include "WeightedDirectedGraph.h"
#include "WeightedUndirectedGraph.h"

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

    void updateNodePosition(NodeItem* nodeItem, const QPointF& point);

signals:

    void sceneModified();

public slots:
    void createGraph(bool, bool);
    void addNode(const QPointF&);
    void addNodeAndEdge(const QPointF&, NodeItem*);
    void addEdge(NodeItem*, NodeItem*);
    void removeNode(NodeItem*);
    void removeEdge(EdgeItem*);
    void editNodeName(const NodeItem*, const QString&);
    void editEdgeWeight(const EdgeItem*, const QString&);
    void moveNode(const NodeItem*, const QPointF&, const QPointF&);

    // clears nodes and edges in both model and view
    void clear();
    // clears nodes and edges only in view
    void clearScene() const;
    // builds scene from already loaded graph
    void buildScene() const;

    // handle methods for GraphScene methods
    void disableScene() const;
    void enableScene() const;
    void resetScene() const;

    void addNodeNoHistory(const QPointF&, unsigned&);
    void addNodeWithIdNoHistory(unsigned, const QPointF&);
    void removeNodeNoHistory(unsigned);
    void addEdgeNoHistory(unsigned, unsigned, int);
    void removeEdgeNoHistory(unsigned, unsigned);
    void setEdgeWeightNoHistory(unsigned, unsigned, int, const QString&);
    void setNodeNameNoHistory(unsigned, const QString&);
    void removeEdgeNoHistoryById(unsigned);
    void addEdgeNoHistoryById(unsigned);
    void restoreEdgeNoHistory(unsigned, unsigned, unsigned, int);
    void setEdgeWeightNoHistoryById(unsigned, int, const QString&);
    void moveNodeNoHistory(unsigned, const QPointF&);
    void clearNoHistory();

private:
    std::shared_ptr<Graph>      m_graph;
    std::unique_ptr<GraphScene> m_scene     = std::make_unique<GraphScene>();
    QUndoStack*                 m_undoStack = nullptr;

    void connectScene() const;
};

#endif // GRAPH_CONTROLLER_H
