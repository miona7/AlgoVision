#ifndef UNDIRECTED_EDGEITEM_H
#define UNDIRECTED_EDGEITEM_H

#include <EdgeItem.h>


class UndirectedEdgeItem : public EdgeItem {
public:
    UndirectedEdgeItem(Edge *modelEdge, NodeItem *sourceNode, NodeItem *destNode);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QPainterPath edgePath() const override;


private:
    qreal m_skewness{10.0};
    qreal m_arrowSize{10.0};
};

#endif // UNDIRECTED_EDGEITEM_H
