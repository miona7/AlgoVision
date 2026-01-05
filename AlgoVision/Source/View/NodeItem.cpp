#include "NodeItem.h"
#include <QPen>
#include <QPainter>

NodeItem::NodeItem(Node* modelNode)
    : m_modelNode(modelNode) {
    setFlags(ItemIsMovable | ItemIsSelectable);
    setZValue(-1);
    setPos(modelNode->getPosition().first, modelNode->getPosition().second);
}

Node *NodeItem::modelNode() const {
    return m_modelNode;
}

void NodeItem::setModelNode(Node *newModelNode) {
    m_modelNode = newModelNode;
}

QRectF NodeItem::boundingRect() const {
    return QRectF(-m_radius - m_borderWidth,
                  -m_radius - m_borderWidth,
                  2 * (m_radius + m_borderWidth),
                  2 * (m_radius + m_borderWidth));
}

QPainterPath NodeItem::shape() const {
    QPainterPath path;
    path.addEllipse(-m_radius, -m_radius, 2 * m_radius, 2 * m_radius);
    return path;
}

void NodeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QPen pen(Qt::black, m_borderWidth);
    QBrush brush(Qt::green);

    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawEllipse(QRectF(-m_radius, -m_radius, 2 * m_radius, 2 * m_radius));
}
