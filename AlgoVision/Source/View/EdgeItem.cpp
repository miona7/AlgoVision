#include "EdgeItem.h"
#include "EditableTextItem.h"
#include "NodeItem.h"

EdgeItem::EdgeItem(Edge* modelEdge, NodeItem* sourceNode, NodeItem* destNode, bool hasWeight)
    : m_modelEdge(modelEdge), m_sourceNode(sourceNode), m_destNode(destNode), m_hasWeight(hasWeight) {
    setFlag(ItemIsSelectable);
    setAcceptedMouseButtons(Qt::LeftButton);

    m_sourceNode->addEdge(this);
    m_destNode->addEdge(this);
    adjust();

    // initialize weigth label (without proper positioning, not responsive to edge movement)
    m_weight = new EditableTextItem(this);
    m_weight->setPlainText(QString::number(modelEdge->getWeight()));
    m_weight->setDefaultTextColor(Qt::black);
    m_weight->setVisible(false);

    if(m_modelEdge != nullptr) {
        m_observerId = m_modelEdge->addObserver([this](Edge&) { this->onEdgeUpdated(); });
    }
}

EdgeItem::~EdgeItem() {
    if(m_modelEdge != nullptr && m_observerId != 0) {
        m_modelEdge->removeObserver(m_observerId);
        m_observerId = 0;
        m_modelEdge  = nullptr; // sprecavamo ponovno pozivanje
    }

    m_sourceNode->removeEdge(this);
    m_destNode->removeEdge(this);
}

void EdgeItem::adjust() {
    QLineF line(mapFromItem(m_sourceNode, 0, 0), mapFromItem(m_destNode, 0, 0));
    qreal  length     = line.length();
    qreal  nodeRadius = m_sourceNode->radius();

    prepareGeometryChange();

    if(length > 2.0 * nodeRadius) {
        QPointF edgeOffset((line.dx() * nodeRadius) / length, (line.dy() * nodeRadius) / length);
        m_sourcePoint = line.p1() + edgeOffset;
        m_destPoint   = line.p2() - edgeOffset;
    } else {
        m_sourcePoint = m_destPoint = line.p1();
    }
}

Edge* EdgeItem::modelEdge() const {
    return m_modelEdge;
}

void EdgeItem::setModelEdge(Edge* newModelEdge) {
    m_modelEdge = newModelEdge;
}

bool EdgeItem::hasWeight() const {
    return m_hasWeight;
}

void EdgeItem::setHasWeight(bool newHasWeight) {
    m_hasWeight = newHasWeight;
}

QPointF EdgeItem::getEdgeCenter() const {
    return edgePath().pointAtPercent(0.5);
}

// remove edge by clicking on it
void EdgeItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    emit edgeSelected(this);
}

const QColor EdgeItem::calculateColor() const {
    if(m_modelEdge == nullptr) {
        return Qt::black; // fallback ako grana ne postoji
    }

    switch(m_modelEdge->getState()) {
    case EdgeState::Examined:
        return Qt::blue;
    case EdgeState::Relaxed:
        return Qt::yellow;
    case EdgeState::Selected:
        return Qt::red;
    case EdgeState::InPath:
        return Qt::green;
    default:
        return Qt::black;
    }
}

void EdgeItem::onEdgeUpdated() {
    if(scene() == nullptr) {
        return;
    }
    update();
}
