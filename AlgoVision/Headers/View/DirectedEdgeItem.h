#ifndef DIRECTED_EDGEITEM_H
#define DIRECTED_EDGEITEM_H

#include <EdgeItem.h>

class DirectedEdgeItem : public EdgeItem {

public:
    DirectedEdgeItem(Edge *modelEdge, NodeItem *sourceNode, NodeItem *destNode);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QPainterPath edgePath() const override;

private:
    qreal m_skewness{70.0};
    qreal m_arrowSize{20.0};
    qreal m_arrowAngle{M_PI / 6.0};

    QPainterPath arrowPath(const QPainterPath& edgePath) const;
    QPointF calculateNormal() const;
};

#endif // DIRECTED_EDGEITEM
