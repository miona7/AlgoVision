#include "GraphView.h"

#include <qevent.h>
#include <QScrollBar>



GraphView::GraphView(QWidget* parent) : QGraphicsView(parent) {
    init();
}

GraphView::GraphView(QGraphicsScene* scene, QWidget* parent) : QGraphicsView(scene, parent) {
    init();
}

GraphView::State GraphView::state() const{
    return m_state;
}

void GraphView::setState(GraphView::State newState){
    m_state = newState;
}

void GraphView::resetState() {
    m_state = GraphView::State::IDLE;
}


void GraphView::mousePressEvent(QMouseEvent* event) {
    if (m_state == GraphView::State::PAN_IDLE) {
        // pan init
        m_state = GraphView::State::PAN_ACTIVE;
        m_lastMousePos = event->pos();
        setCursor(Qt::ClosedHandCursor);

        event->accept();
        return;
    }

    if (m_state != GraphView::State::IDLE) {
        event->accept();
        return;
    }

    QGraphicsView::mousePressEvent(event);
}

void GraphView::mouseMoveEvent(QMouseEvent* event) {
    if (m_state == GraphView::State::PAN_ACTIVE) {
        // pan update logic (u koordinatama scene, da ne zavise proracuni od zoom-a)
        QPoint delta = event->pos() - m_lastMousePos;
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
        m_lastMousePos = event->pos();

        event->accept();
        return;
    }

    if (m_state != GraphView::State::IDLE) {
        event->accept();
        return;
    }

    QGraphicsView::mouseMoveEvent(event);
}

void GraphView::mouseReleaseEvent(QMouseEvent* event) {
    if (m_state == GraphView::State::PAN_ACTIVE) {
        m_state = GraphView::State::PAN_IDLE;
        setCursor(Qt::ArrowCursor);
        event->accept();
        return;
    }

    if (m_state != GraphView::State::IDLE) {
        event->accept();
        return;
    }

    QGraphicsView::mouseReleaseEvent(event);
}

void GraphView::mouseDoubleClickEvent(QMouseEvent* event) {
    if (m_state != GraphView::State::IDLE) {
        event->accept();
        return;
    }

    QGraphicsView::mouseDoubleClickEvent(event);
}

void GraphView::init() {
    setRenderHint(QPainter::Antialiasing);
    setAlignment(Qt::AlignCenter);
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    setResizeAnchor(QGraphicsView::AnchorViewCenter);
}
