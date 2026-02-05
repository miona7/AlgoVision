#include "UndirectedEdgeItem.h"

UndirectedEdgeItem::UndirectedEdgeItem(Edge* modelEdge, NodeItem* sourceNode, NodeItem* destNode,
                                       bool hasWeight)
    : EdgeItem(modelEdge, sourceNode, destNode, hasWeight) {
}

QPainterPath UndirectedEdgeItem::edgePath() const {
    QPainterPath path;
    path.moveTo(m_sourcePoint);
    path.lineTo(m_destPoint);
    return path;
}

QRectF UndirectedEdgeItem::boundingRect() const {
    qreal offset = AppConstants::BaseEdgeWidth * AppConstants::NodeScale;
    return edgePath().boundingRect().adjusted(-offset, -offset, offset, offset);
}

QPainterPath UndirectedEdgeItem::shape() const {
    QPainterPathStroker stroker;
    qreal offset = AppConstants::BaseEdgeWidth * AppConstants::NodeScale + m_shapeStroke;
    stroker.setWidth(offset);
    return stroker.createStroke(edgePath());
}

void UndirectedEdgeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                               QWidget* widget) {
    // QPen pen(Qt::black, m_penWidth);
    auto color = calculateColor();
    QPen pen(color);
    pen.setWidthF(AppConstants::BaseEdgeWidth * AppConstants::NodeScale);
    painter->setPen(pen);
    painter->drawPath(edgePath());
}
