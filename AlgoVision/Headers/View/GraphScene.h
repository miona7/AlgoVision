#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <QGraphicsScene>
#include <map>

#include "NodeItem.h"
#include "Edge.h"

class GraphScene : public QGraphicsScene {
    Q_OBJECT

public:
    explicit GraphScene(QObject* = nullptr);

    enum class State { ADD, REMOVE, EDIT };

    void setState(GraphScene::State);

    // vraca scenu na pocetno stanje (koje je ADD) sa resetovanim pomocnim privatnim clanicama
    void resetScene();

    // pored ciscenja scene, dodatno brise mapu nodeItem-a, i vraca scenu na pocetno stanje
    void clear();

    void addNode(Node*);
    void addEdge(Edge*, bool, bool);
    void removeNode(NodeItem*);
    void removeEdge(EdgeItem*);

    NodeItem* findNodeItemById(const unsigned) const;

signals:
    void addNodeRequest(const QPointF&);
    void addEdgeRequest(NodeItem*, NodeItem*);
    void removeNodeRequest(NodeItem*);
    void removeEdgeRequest(EdgeItem*);

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
    State             m_state {State::ADD};

    // cuva NodeItem-e, kako bi mogli da se koriste prilikom pravljenja pogleda grafa od vec ucitanog modela
    // sluzi kao veza modela cvora sa odgovarajucim pogledom
    // ne poseduje NodeItem-e, samo ih koristi
    std::map<unsigned, NodeItem*> m_nodeItems;

    void      selectNode(NodeItem*);
};

#endif // GRAPHSCENE_H
