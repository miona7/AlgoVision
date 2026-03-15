#ifndef EDGEITEM_H
#define EDGEITEM_H

#include <QFont>
#include <QGraphicsObject>
#include <QGraphicsSceneEvent>

#include "Edge.h"
#include "EditableTextItem.h"

class NodeItem;

class EdgeItem : public QGraphicsObject {
    Q_OBJECT

public:
    EdgeItem(Edge*, NodeItem*, NodeItem*, bool = false);
    ~EdgeItem() override;

    void                 initEdgeWeight();
    void                 adjust();
    void                 adjustPointsGeometry();
    void                 adjustWeightGeometry() const;
    virtual QPainterPath edgePath() const                                                     = 0;
    QRectF               boundingRect() const override                                        = 0;
    QPainterPath         shape() const override                                               = 0;
    void                 paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override = 0;

    void connectNodes();
    void disconnectNodes();

    Edge* modelEdge() const;
    void  setModelEdge(Edge*);

    bool hasWeight() const;
    void setHasWeight(bool);

    EditableTextItem* weight() const;
    void              setWeight(EditableTextItem*);

    void updateSize();

signals:
    void edgeSelected(EdgeItem*);
    void editEdgeWeightRequest(const EdgeItem*, const QString&) const;

private slots:
    void onEdgeWeightChanged(const QString&) const;

protected:
    Edge*             m_modelEdge;
    EditableTextItem* m_weight{nullptr};
    bool              m_hasWeight;

    NodeItem* m_sourceNode;
    NodeItem* m_destNode;
    QPointF   m_sourcePoint;
    QPointF   m_destPoint;

    qreal m_penWidth{2};
    qreal m_shapeStroke{20.0};

    unsigned m_observerId{0};

    void         mousePressEvent(QGraphicsSceneMouseEvent*) override;
    const QColor calculateColor() const;
    void         onEdgeUpdated();

    QPointF getEdgeCenter() const;
    QPointF calculateNormal() const;
    QPointF getWeightPosition() const;

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent*) override;
};

#endif // EDGEITEM_H
