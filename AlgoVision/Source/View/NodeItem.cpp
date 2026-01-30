#include <EditableTextItem.h>
#include <QGraphicsSceneEvent>
#include <QPainter>
#include <QPen>
#include <QGraphicsSceneMouseEvent>

#include "EdgeItem.h"
#include "NodeItem.h"
#include "AppConstants.h"

NodeItem::NodeItem(Node* modelNode) : m_modelNode(modelNode) {
    setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);
    setAcceptedMouseButtons(Qt::LeftButton);
    setZValue(-1);
    setPos(modelNode->getPosition().first, modelNode->getPosition().second);

    // node name
    m_label = new EditableTextItem(this);
    m_label->setPlainText(m_modelNode->getName());
    m_label->setTextWidth(2 * m_radius);
    m_label->setDefaultTextColor(Qt::black);
    m_label->setCenter(0.0, 0.0);
    m_label->centerText();
    connect(m_label, &EditableTextItem::textCommited, this, &NodeItem::onNameChanged);

    // observer
    if(m_modelNode != nullptr) {
        m_observerId = m_modelNode->addObserver([this](Node&) { this->onNodeUpdated(); });
    }
    updateSize();

}

NodeItem::~NodeItem() {
    if(m_modelNode != nullptr && m_observerId != 0) {
        m_modelNode->removeObserver(m_observerId);
        m_observerId = 0;
        m_modelNode  = nullptr;
    }
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
        for(auto* edge: m_edges) {
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
    if(!m_hasChangePosition && !m_ignoreNextMouseRealese) {
        m_nodeSelected = !m_nodeSelected;
        update();
        emit nodeSelected(this);
    }

    if(m_ignoreNextMouseRealese) {
        m_ignoreNextMouseRealese = false;
    }

    QGraphicsItem::mouseReleaseEvent(event);
}

/*
    mouseDoubleClickEvent se sastoji iz naredna 4 dogadjaja ovim redom:
        1. mousePressEvent
        2. mouseRealeseEvent
        3. mouseDoubleClickEvent
        4. mouseReleaseEvent

 Problem je sto mouseReleaseEvent ima select logiku koja se koristi i pri dodavanju grana,
 a to ne zelimo da se desava kada hocemo da izvrsimo rename logiku sa duplim klikom.

 Ne postoji nacin kako da se prvi mouseRealeseEvent spreci jer se ne zna da li ce se izvrsiti
 mouseDoubleClickEvent ili ne, pa ga izvrsavamo, ali ako se desi mouseDoubleClickEvent ponavo
 izvrsavamo logiku mouseReleaseEvent-a, jer sva logika i u nodeItem i u GrahiScene u okviru
selectNode() metode je takva da se sa ponovnim izvrsavanjem sa istim argumentima ponistava.

 ignoreNextMouseRelease flag omogucuje da se ignorise drugi mouseReleaseEvent koji se desava
nakon mouseDoubleClickEvent-a
*/
void NodeItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) {
    event->accept();
    m_label->startEditing();
    m_ignoreNextMouseRealese = true;

    m_nodeSelected = !m_nodeSelected;
    update();
    emit nodeSelected(this);
}

void NodeItem::onNameChanged(const QString& name) const {
    emit editNodeNameRequest(this, name);
}

EditableTextItem* NodeItem::label() const {
    return m_label;
}

void NodeItem::setLabel(EditableTextItem* newLabel) {
    m_label = newLabel;
}

const QSet<EdgeItem*>& NodeItem::edges() const {
    return m_edges;
}

void NodeItem::setNodeSelected(bool newNodeSelected) {
    m_nodeSelected = newNodeSelected;
    update();
}


void NodeItem::updateSize() {

    prepareGeometryChange();
    m_radius = AppConstants::defaultRadius * AppConstants::NodeScale;
    m_borderWidth = AppConstants::defaultBorderWidth * AppConstants::NodeScale;

    update();

    for(auto edge : m_edges) {
        if(edge){
            edge->adjust();
        }
    }

    QFont f = m_label->font();
    f.setPointSizeF(AppConstants::BaseFontSize * AppConstants::NodeScale);
    m_label->setFont(f);

    m_label->setTextWidth(2 * m_radius);
    m_label->centerText();


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

/*
void NodeItem::setRadius(qreal newRadius) {
    m_radius = newRadius;
}
*/
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
    case NodeState::UpdatedDistance:
        return Qt::darkMagenta;
    case NodeState::InPath:
        return Qt::green;
    case NodeState::TopologicalOrder:
        return Qt::darkYellow;
    case NodeState::AssignedComponent:
        return Qt::cyan;
    default:
        return Qt::lightGray;
        // return Qt::green;
    }
}

void NodeItem::onNodeUpdated() {
    if(scene() == nullptr) {
        return;
    }
    update();
}
