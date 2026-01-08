#include <DirectedEdgeItem.h>
#include <QPainter>
#include <QPen>


DirectedEdgeItem::DirectedEdgeItem(Edge *modelEdge, NodeItem *sourceNode, NodeItem *destNode)
    : EdgeItem(modelEdge, sourceNode, destNode) {}

QRectF DirectedEdgeItem::boundingRect() const {
    qreal offset = m_penWidth;
    return edgePath().boundingRect().adjusted(-offset, -offset, offset, offset);
}

QPainterPath DirectedEdgeItem::shape() const {
    QPainterPathStroker stroker;
    qreal offset = m_penWidth + 3.0; // bigger offset, so the click would be easier, more UX friendly
    stroker.setWidth(offset);
    return stroker.createStroke(edgePath());
}

void DirectedEdgeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QPen pen(Qt::black, m_penWidth);
    painter->setPen(pen);
    painter->drawPath(edgePath());
}

QPainterPath DirectedEdgeItem::edgePath() const {
    QPainterPath path;
    QPointF normal = calculateNormal();

    if (!(qFuzzyCompare(normal.x(), 0.0) && qFuzzyCompare(normal.y(), 0.0))) {
        path.moveTo(m_sourcePoint);
        QPointF middle = (m_sourcePoint + m_destPoint) / 2.0 - normal * m_skewness;
        path.quadTo(middle, m_destPoint);
    }

    return path;
}

QPainterPath DirectedEdgeItem::arrowPath() const {
    //TODO: implement arrow
    return QPainterPath();
}

QPointF DirectedEdgeItem::calculateNormal() const {
    QPointF line(m_destPoint.x() - m_sourcePoint.x(), m_destPoint.y() - m_sourcePoint.y());
    QPointF normal(-line.y(), line.x());
    qreal length = std::hypot(normal.x(), normal.y());

    if (qFuzzyCompare(length, 0.0)) {
        normal *= 0.0;
    } else {
        normal /= length;
    }

    return normal;
}
