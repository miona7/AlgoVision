#ifndef EDGEITEM_H
#define EDGEITEM_H

#include <QGraphicsObject>

#include "Edge.h"

class NodeItem;

class EdgeItem : public QGraphicsObject {
    Q_OBJECT

public:
    EdgeItem(Edge*, NodeItem*, NodeItem*);
    virtual ~EdgeItem() override;

    void                 adjust();
    virtual QPainterPath edgePath() const                                                     = 0;
    QRectF               boundingRect() const override                                        = 0;
    QPainterPath         shape() const override                                               = 0;
    void                 paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override = 0;

    Edge* modelEdge() const;
    void  setModelEdge(Edge*);

signals:
    void edgeSelected(EdgeItem*);

protected:
    Edge* m_modelEdge;
    // std::shared_ptr<Edge> m_modelEdge;

    NodeItem* m_sourceNode;
    NodeItem* m_destNode;
    QPointF   m_sourcePoint;
    QPointF   m_destPoint;

    qreal m_penWidth {2};
    qreal m_shapeStroke {20.0};

    unsigned m_observerId = 0;

    void         mousePressEvent(QGraphicsSceneMouseEvent*) override;
    const QColor calculateColor() const;
    void         onEdgeUpdated();
};

#endif // EDGEITEM_H
