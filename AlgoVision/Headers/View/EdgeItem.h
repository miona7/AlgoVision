#ifndef EDGEITEM_H
#define EDGEITEM_H

#include <Edge.h>
#include <QGraphicsObject>

class NodeItem;

class EdgeItem : public QGraphicsObject {
    Q_OBJECT

public:
    EdgeItem(Edge* modelEdge, NodeItem* sourceNode, NodeItem* destNode);
    virtual ~EdgeItem();

    void                 adjust();
    virtual QPainterPath edgePath() const                = 0;
    QRectF               boundingRect() const override   = 0;
    QPainterPath         shape() const override          = 0;
    void                 paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                               QWidget* widget) override = 0;

    Edge* modelEdge() const;
    void  setModelEdge(Edge* newModelEdge);

signals:
    void edgeSelected(EdgeItem* self);

protected:
    Edge* m_modelEdge;

    NodeItem* m_sourceNode;
    NodeItem* m_destNode;
    QPointF   m_sourcePoint;
    QPointF   m_destPoint;

    qreal m_penWidth {2};
    qreal m_shapeStroke {20.0};

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
};

#endif // EDGEITEM_H
