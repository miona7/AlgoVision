#include <EdgeItem.h>
#include <NodeItem.h>
#include <QGraphicsSceneEvent>
#include <QPainter>
#include <QPen>

NodeItem::NodeItem(Node* modelNode) : m_modelNode(modelNode) {
    setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);
    setAcceptedMouseButtons(Qt::LeftButton);
    setZValue(-1);
    setPos(modelNode->getPosition().first, modelNode->getPosition().second);

    // observer
    if(m_modelNode != nullptr) {
        m_observerId = m_modelNode->addObserver(
            [this](Node&) {
                this->onNodeUpdated();
            }
        );
    }
}

NodeItem::~NodeItem() {
    if(m_modelNode != nullptr) {
        m_modelNode->removeObserver(m_observerId);
    }

    for(auto edge: m_edges) {
        delete edge;
    }

    m_edges.clear();
}

Node* NodeItem::modelNode() const {
    return m_modelNode;
}

void NodeItem::setModelNode(Node* newModelNode) {
    m_modelNode = newModelNode;
}

QRectF NodeItem::boundingRect() const {
    return QRectF(-m_radius - m_borderWidth, -m_radius - m_borderWidth,
                  2 * (m_radius + m_borderWidth), 2 * (m_radius + m_borderWidth));
}

QPainterPath NodeItem::shape() const {
    QPainterPath path;
    path.addEllipse(-m_radius, -m_radius, 2 * m_radius, 2 * m_radius);
    return path;
}

void NodeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    QPen pen(Qt::black, m_borderWidth);

    /*
    if(m_nodeSelected) {
        pen.setColor(Qt::red);
        pen.setWidth(m_borderWidth + 2);
    }
    */

    QBrush brush(calculateColor());

    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawEllipse(QRectF(-m_radius, -m_radius, 2 * m_radius, 2 * m_radius));
}

QVariant NodeItem::itemChange(GraphicsItemChange change, const QVariant& value) {
    switch(change) {
    case QGraphicsItem::ItemPositionHasChanged:
        for(auto edge: m_edges) {
            edge->adjust();
        }

        break;
    default:
        break;
    }

    return QGraphicsItem::itemChange(change, value);
}

// test: right click on node delete itself
void NodeItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    m_hasChangePosition = false;
    QGraphicsItem::mousePressEvent(event);
}

void NodeItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    m_hasChangePosition = true;
    QGraphicsItem::mouseMoveEvent(event);
}

void NodeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    if(!m_hasChangePosition) {
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

void NodeItem::addEdge(EdgeItem* edgeItem) {
    m_edges.insert(edgeItem);
}

void NodeItem::removeEdge(EdgeItem* edgeItem) {
    m_edges.remove(edgeItem);
}

qreal NodeItem::radius() const {
    return m_radius;
}

void NodeItem::setRadius(qreal newRadius) {
    m_radius = newRadius;
}

const QColor NodeItem::calculateColor() const {
    // return (!m_nodeSelected) ? Qt::green : Qt::red;

    if(m_nodeSelected) {
        return Qt::red;
    }

    switch(m_modelNode->getState()) {
    case NodeState::Active:
        return Qt::yellow;
    case NodeState::Visited:
        return Qt::blue;
    case NodeState::InPath:
        return Qt::green;
    case NodeState::TopologicalOrder:
        return Qt::darkYellow;
    case NodeState::AssignedComponent:
        return Qt::cyan;
    default:
        return Qt::lightGray;
    }
}

void NodeItem::onNodeUpdated() {
    update();
}
