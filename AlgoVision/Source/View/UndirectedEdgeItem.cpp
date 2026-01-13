#include <QPainter>
#include <QPen>
#include <UndirectedEdgeItem.h>

UndirectedEdgeItem::UndirectedEdgeItem(Edge* modelEdge, NodeItem* sourceNode, NodeItem* destNode)
    : EdgeItem(modelEdge, sourceNode, destNode) {
}

QPainterPath UndirectedEdgeItem::edgePath() const {
    QPainterPath path;
    path.moveTo(m_sourcePoint);
    path.lineTo(m_destPoint);
    return path;
}

QRectF UndirectedEdgeItem::boundingRect() const {
    qreal offset = m_penWidth;
    return edgePath().boundingRect().adjusted(-offset, -offset, offset, offset);
}

QPainterPath UndirectedEdgeItem::shape() const {
    QPainterPathStroker stroker;
    qreal               offset =
        m_penWidth + m_shapeStroke; // bigger offset, so the click would be easier, more UX friendly
    stroker.setWidth(offset);
    return stroker.createStroke(edgePath());
}

void UndirectedEdgeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                               QWidget* widget) {
    QPen pen(Qt::black, m_penWidth);
    painter->setPen(pen);
    painter->drawPath(edgePath());
}
