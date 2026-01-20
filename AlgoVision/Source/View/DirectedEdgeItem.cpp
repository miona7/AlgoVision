#include <QPainter>
#include <QPen>

#include "DirectedEdgeItem.h"

DirectedEdgeItem::DirectedEdgeItem(Edge* modelEdge, NodeItem* sourceNode, NodeItem* destNode, bool hasWeight)
    : EdgeItem(modelEdge, sourceNode, destNode, hasWeight) {
}

QRectF DirectedEdgeItem::boundingRect() const {
    qreal        offset = m_penWidth;
    QPainterPath edge   = edgePath();
    edge.addPath(arrowPath(edge));
    return edge.boundingRect().adjusted(-offset, -offset, offset, offset);
}

QPainterPath DirectedEdgeItem::shape() const {
    QPainterPathStroker stroker;
    qreal               offset =
        m_penWidth + m_shapeStroke; // bigger offset, so the click would be easier, more UX friendly
    stroker.setWidth(offset);

    QPainterPath edge  = edgePath();
    QPainterPath arrow = arrowPath(edge);
    QPainterPath shape = stroker.createStroke(edge);
    shape.addPath(arrow);

    return shape;
}

void DirectedEdgeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                             QWidget* widget) {
    QLineF line(m_sourcePoint, m_destPoint);
    if(qFuzzyCompare(line.length(), 0.0)) {
        return;
    }

    QPainterPath edge = edgePath();
    // QPen         pen(Qt::black, m_penWidth);
    auto color = calculateColor();
    QPen pen(color, m_penWidth);
    painter->setPen(pen);
    painter->drawPath(edge);

    QBrush brush(color);
    painter->setBrush(brush);
    painter->drawPath(arrowPath(edge));
}

QPainterPath DirectedEdgeItem::edgePath() const {
    QPainterPath path;
    QPointF      normal = calculateNormal();

    if(!(qFuzzyCompare(normal.x(), 0.0) && qFuzzyCompare(normal.y(), 0.0))) {
        path.moveTo(m_sourcePoint);
        QPointF middle = (m_sourcePoint + m_destPoint) / 2.0 - normal * m_skewness;
        path.quadTo(middle, m_destPoint);
    }

    return path;
}

QPainterPath DirectedEdgeItem::arrowPath(const QPainterPath& edgePath) const {
    QPointF t     = edgePath.pointAtPercent(1.0) - edgePath.pointAtPercent(0.95);
    qreal   angle = std::atan2(t.y(), t.x());

    QPointF p1 = m_destPoint - QPointF(m_arrowSize * std::cos(angle - m_arrowAngle),
                                       m_arrowSize * std::sin(angle - m_arrowAngle));

    QPointF p2 = m_destPoint - QPointF(m_arrowSize * std::cos(angle + m_arrowAngle),
                                       m_arrowSize * std::sin(angle + m_arrowAngle));

    QPainterPath arrowPath;
    arrowPath.moveTo(m_destPoint);
    arrowPath.lineTo(p1);
    arrowPath.lineTo(p2);
    arrowPath.closeSubpath();
    return arrowPath;
}
