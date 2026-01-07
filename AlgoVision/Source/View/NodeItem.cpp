#include "NodeItem.h"
#include <QPen>
#include <QPainter>

NodeItem::NodeItem(Node* modelNode)
    : m_modelNode(modelNode) {
    setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);
    setAcceptedMouseButtons(Qt::LeftButton);
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
    QBrush brush(calculateColor());

    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawEllipse(QRectF(-m_radius, -m_radius, 2 * m_radius, 2 * m_radius));
}

QVariant NodeItem::itemChange(GraphicsItemChange change, const QVariant &value) {
    switch (change) {
    case QGraphicsItem::ItemPositionHasChanged:
        //m_hasChangePosition = true;
        break;
    default:
        break;
    }

    return QGraphicsItem::itemChange(change, value);
}

void NodeItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    m_hasChangePosition = false;
    QGraphicsItem::mousePressEvent(event);
}

void NodeItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    m_hasChangePosition = true;
    QGraphicsItem::mouseMoveEvent(event);
}

void NodeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (!m_hasChangePosition) {
        m_nodeSelected = !m_nodeSelected;
        update();
        emit nodeSelected(this);
    }

    QGraphicsItem::mouseReleaseEvent(event);
}

void NodeItem::setNodeSelected(bool newNodeSelected) {
    m_nodeSelected = newNodeSelected;
    update();
}

qreal NodeItem::radius() const {
    return m_radius;
}

void NodeItem::setRadius(qreal newRadius) {
    m_radius = newRadius;
}

const QColor NodeItem::calculateColor() const {
    return (!m_nodeSelected)? Qt::green : Qt::red;
}
