#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

#include <map>

#include "DirectedEdgeItem.h"
#include "Edge.h"
#include "EdgeItem.h"
#include "EditableTextItem.h"
#include "Node.h"
#include "NodeItem.h"
#include "ThemeManager.h"
#include "UndirectedEdgeItem.h"

class GraphScene : public QGraphicsScene {
    Q_OBJECT

public:
    explicit GraphScene(QObject* = nullptr);

    /*
        ADD -> adding nodes and edges
        REMOVE -> removing nodes and edges
        EDIT -> changing edge weights, and changing node name, which is supported and can be
       activated IDLE -> all scene interactions are disabled
    */
    enum class State { ADD, REMOVE, EDIT, IDLE };

    void updateNodeScalling();

    void setState(GraphScene::State);
    // returns the scene in initial state (clears all selections and ends all edits)
    void resetScene();
    // disables scene interactions (adding, removing, updating and moving elements)
    void disableScene();
    // reenables scene, returns it in state before disable
    void enableScene();

    void clear();

    void addNode(Node*);
    void addEdge(Edge*, bool, bool);
    // additional command that encapsulates adding node and edge into one command that is revertable
    // via undo/redo
    void addNodeAndEdge(Node*, Edge*, bool, bool);
    void removeNode(NodeItem*);
    void removeEdge(EdgeItem*);
    void applyTheme(ThemeManager::Theme theme);

    NodeItem* findNodeItemById(const unsigned) const;
    EdgeItem* findEdgeItemByNodes(unsigned, unsigned) const;
    EdgeItem* findEdgeItemById(unsigned) const;

signals:
    void addNodeRequest(const QPointF&);
    void addNodeAndEdgeRequest(const QPointF&, NodeItem*);
    void addEdgeRequest(NodeItem*, NodeItem*);
    void removeNodeRequest(NodeItem*);
    void removeEdgeRequest(EdgeItem*);
    void moveNodeRequest(const NodeItem*, const QPointF&, const QPointF&);
    void editNodeNameRequest(const NodeItem*, const QString&) const;
    void editEdgeWeightRequest(const EdgeItem*, const QString&) const;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent*) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent*) override;

private slots:
    void onNodeSelectTrigger(NodeItem*);
    void onEdgeSelectTrigger(EdgeItem*);

    // reacts to editabletextitem scene change
    void setEditGraphSceneTrigger(bool, EditableTextItem* = nullptr);

private:
    NodeItem*         m_firstNodeSelect {nullptr};
    EditableTextItem* m_editLabel {nullptr};
    State             m_state {GraphScene::State::ADD};
    State             m_previousState;

    // structure only used as helper for operations, not owning elements
    std::map<unsigned, NodeItem*> m_nodeItems;

    void selectNode(NodeItem*);
};

#endif // GRAPHSCENE_H
