#ifndef EDGEITEM_H
#define EDGEITEM_H

#include <Edge.h>
#include <NodeItem.h>
#include <QGraphicsObject>


class EdgeItem : public QGraphicsObject {
    Q_OBJECT

public:
    EdgeItem(Edge *modelEdge, NodeItem *sourceNode, NodeItem *destNode);
    virtual ~EdgeItem() = default;

    void adjust();
    virtual QPainterPath edgePath() const = 0;

protected:
    QRectF boundingRect() const override = 0;
    QPainterPath shape() const override = 0;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override = 0;

private:
    Edge* m_modelEdge;

    NodeItem* m_sourceNode;
    NodeItem* m_destNode;
    QPointF m_sourcePoint;
    QPointF m_destPoint;

    int m_orientation;
    qreal m_penWidth{2};
};

#endif // EDGEITEM_H
