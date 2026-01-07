#include "EdgeItem.h"
#include "NodeItem.h"

EdgeItem::EdgeItem(Edge *modelEdge, NodeItem *sourceNode, NodeItem *destNode)
    : m_modelEdge(modelEdge), m_sourceNode(sourceNode), m_destNode(destNode) {
    setFlag(ItemIsSelectable);
    setAcceptedMouseButtons(Qt::LeftButton);

    m_sourceNode->addEdge(this);
    m_destNode->addEdge(this);
    adjust();
}

void EdgeItem::adjust() {
    QLineF line(mapFromItem(m_sourceNode, 0, 0), mapFromItem(m_destNode, 0, 0));
    qreal length = line.length();
    qreal nodeRadius = m_sourceNode->radius();

    prepareGeometryChange();

    if (length > 2.0 * nodeRadius) {
        QPointF edgeOffset((line.dx() * nodeRadius) / length, (line.dy() * nodeRadius) / length);
        m_sourcePoint = line.p1() + edgeOffset;
        m_destPoint = line.p2() - edgeOffset;
    } else {
        m_sourcePoint = m_destPoint = line.p1();
    }
}
