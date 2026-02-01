#include "GraphView.h"

#include <qevent.h>



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
    switch (m_state) {
    case GraphView::State::PAN: {
        // pan logic

        event->accept();
        break;
    }

    case GraphView::State::ZOOM_IN: {
        // zoom in logic

        event->accept();
        break;
    }

    case GraphView::State::ZOOM_OUT: {
        // zoom out logic

        event->accept();
        break;
    }

    default:
        QGraphicsView::mousePressEvent(event);
        break;
    }
}

void GraphView::mouseMoveEvent(QMouseEvent* event) {
    if (m_state != GraphView::State::IDLE) {
        event->accept();
        return;
    }

    QGraphicsView::mouseMoveEvent(event);
}

void GraphView::mouseReleaseEvent(QMouseEvent* event) {
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
    setAlignment(Qt::AlignCenter);
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    setResizeAnchor(QGraphicsView::AnchorViewCenter);
}
