#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <Graph.h>
#include <NodeItem.h>
#include <QGraphicsScene>

class GraphScene : public QGraphicsScene {
    Q_OBJECT

public:
    explicit GraphScene(Graph*, QObject* = nullptr);

    enum class State { ADD, REMOVE, EDIT };

    void setState(GraphScene::State);
    void resetScene();

    Graph* getGraphRaw() const;
    std::shared_ptr<Graph> getGraphShared() const;

    void addNode(Node*);
    void addEdge(Edge*, NodeItem*, NodeItem*, bool, bool);
    void removeNode(NodeItem*);
    void removeEdge(EdgeItem*);

signals:
    void addNodeRequest(const QPointF& point);
    void addEdgeRequest(NodeItem* source, NodeItem* dest);
    void removeNodeRequest(NodeItem* nodeItem);
    void removeEdgeRequest(EdgeItem* edgeItem);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent*) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;

private slots:
    void onNodeSelectTrigger(NodeItem*);
    void onEdgeSelectTrigger(EdgeItem*);

    // reacts to editabletextitem scene change
    void setEditGraphSceneTrigger(bool edit, EditableTextItem* label = nullptr);

private:
    NodeItem*         m_firstNodeSelect {nullptr};
    EditableTextItem* m_editLabel {nullptr};
    State             m_state {State::ADD};
    Graph*            m_graph {nullptr};

    void      selectNode(NodeItem*);
    EdgeItem* makeEdgeItem(Edge*, NodeItem*, NodeItem*, bool, bool) const;
};

#endif // GRAPHSCENE_H
