#include "EdgeItem.h"
#include "NodeItem.h"

EdgeItem::EdgeItem(Edge* modelEdge, NodeItem* sourceNode, NodeItem* destNode, bool hasWeight)
    : m_modelEdge(modelEdge), m_sourceNode(sourceNode), m_destNode(destNode),
      m_hasWeight(hasWeight) {
    setFlag(ItemIsSelectable);
    setAcceptedMouseButtons(Qt::LeftButton);

    connectNodes();

    if(m_modelEdge != nullptr) {
        m_observerId = m_modelEdge->addObserver([this](Edge&) { this->onEdgeUpdated(); });
    }
}

EdgeItem::~EdgeItem() {
    if(m_modelEdge != nullptr && m_observerId != 0) {
        m_modelEdge->removeObserver(m_observerId);
        m_observerId = 0;
        m_modelEdge  = nullptr; // prevent re-calling
    }
}

void EdgeItem::initEdgeWeight() {
    if(m_hasWeight) {
        m_weight = new EditableTextItem(this);
        m_weight->setPlainText(QString::number(m_modelEdge->getWeight()));
        m_weight->setDefaultTextColor(Qt::black);
        connect(m_weight, &EditableTextItem::textCommited, this, &EdgeItem::onEdgeWeightChanged);
    }
}

void EdgeItem::adjust() {

    QLineF line(mapFromItem(m_sourceNode, 0, 0), mapFromItem(m_destNode, 0, 0));
    qreal  length = line.length();

    qreal r = m_sourceNode->radius() + AppConstants::EdgePadding * AppConstants::NodeScale;

    prepareGeometryChange();

    if(length > 2 * r) {
        QPointF offset((line.dx() * r) / length, (line.dy() * r) / length);
        m_sourcePoint = line.p1() + offset;
        m_destPoint   = line.p2() - offset;
    } else {
        m_sourcePoint = m_destPoint = line.p1();
    }
    adjustPointsGeometry();
}

void EdgeItem::connectNodes() {
    if(m_sourceNode != nullptr) {
        m_sourceNode->addEdge(this);
    }

    if(m_destNode != nullptr) {
        m_destNode->addEdge(this);
    }
}

void EdgeItem::disconnectNodes() {
    if(m_sourceNode != nullptr) {
        m_sourceNode->removeEdge(this);
    }

    if(m_destNode != nullptr) {
        m_destNode->removeEdge(this);
    }
}

void EdgeItem::adjustPointsGeometry() {
    QLineF line(mapFromItem(m_sourceNode, 0, 0), mapFromItem(m_destNode, 0, 0));
    qreal  length     = line.length();
    qreal  nodeRadius = m_sourceNode->radius();

    if(length > 2.0 * nodeRadius) {
        QPointF edgeOffset((line.dx() * nodeRadius) / length, (line.dy() * nodeRadius) / length);
        m_sourcePoint = line.p1() + edgeOffset;
        m_destPoint   = line.p2() - edgeOffset;

        if(m_hasWeight) {
            m_weight->setVisible(true);
            adjustWeightGeometry();
        }
    } else {
        m_sourcePoint = m_destPoint = line.p1();

        if(m_hasWeight) {
            m_weight->setVisible(false);
        }
    }
}

void EdgeItem::adjustWeightGeometry() const {
    if(!m_hasWeight) {
        return;
    }

    constexpr qreal factor = 1.5;

    QPainterPathStroker stroker;
    stroker.setWidth(m_penWidth + 2.0);
    QPainterPath edge = stroker.createStroke(edgePath());

    auto pos    = getWeightPosition();
    auto normal = calculateNormal();

    for(;;) {
        m_weight->setCenter(pos);
        m_weight->centerText();

        auto rect = m_weight->mapRectToParent(m_weight->boundingRect());

        if(!edge.intersects(rect)) {
            break;
        }

        pos -= factor * normal;
    }
}

void EdgeItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) {
    event->accept();

    if(m_hasWeight) {
        m_weight->startEditing();
    }
}

void EdgeItem::onEdgeWeightChanged(const QString& name) const {
    emit editEdgeWeightRequest(this, name);
}

EditableTextItem* EdgeItem::weight() const {
    return m_weight;
}

void EdgeItem::setWeight(EditableTextItem* newWeight) {
    m_weight = newWeight;
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

QPointF EdgeItem::calculateNormal() const {
    QPointF line(m_destPoint.x() - m_sourcePoint.x(), m_destPoint.y() - m_sourcePoint.y());
    QPointF normal(-line.y(), line.x());
    qreal   length = std::hypot(normal.x(), normal.y());

    if(qFuzzyCompare(length, 0.0)) {
        normal *= 0.0;
    } else {
        normal /= length;
    }

    return normal;
}

QPointF EdgeItem::getWeightPosition() const {
    auto  normal = calculateNormal();
    auto  center = getEdgeCenter();
    qreal offset = 10;
    return QPointF(center.x() - offset * normal.x(), center.y() - offset * normal.y());
}

// remove edge by clicking on it
void EdgeItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    emit edgeSelected(this);
}

const QColor EdgeItem::calculateColor() const {
    if(m_modelEdge == nullptr) {
        return Qt::black; // fallback if edge does not exist
    }

    switch(m_modelEdge->getState()) {
    case EdgeState::Examined:
        return Qt::blue;
    case EdgeState::Relaxed:
        return Qt::yellow;
    case EdgeState::Selected:
        return Qt::red;
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

void EdgeItem::updateSize() {
    if(!m_hasWeight || m_weight == nullptr) {
        return;
    }

    QFont f = m_weight->font();
    f.setPointSizeF(AppConstants::BaseFontSize * AppConstants::NodeScale);
    m_weight->setFont(f);

    adjustWeightGeometry();
}
