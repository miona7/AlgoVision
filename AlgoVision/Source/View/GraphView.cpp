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
    switch (m_state) {
        case GraphView::State::PAN_IDLE: {
            setCursor(Qt::OpenHandCursor);
            break;
        }

        case GraphView::State::PAN_ACTIVE: {
            setCursor(Qt::ClosedHandCursor);
            break;
        }

        case GraphView::State::IDLE:
        default: {
            setCursor(Qt::ArrowCursor);
            break;
        }
    }
}

void GraphView::resetState() {
    setState(GraphView::State::IDLE);
}

void GraphView::zoomIn() {
    if (transform().m11() < 3.0) {
        scale(m_zoomFactor, m_zoomFactor);
    }
}

void GraphView::zoomOut() {
    if (transform().m11() > 0.3) {
        scale(1.0 / m_zoomFactor, 1.0 / m_zoomFactor);
    }
}


void GraphView::mousePressEvent(QMouseEvent* event) {
    // desni klik je podrazumevana precica za pan, koji se odmah izvrsava drzanjem desnog klika
    // potrebno je da upamtimo staro stanje da bi znali na sta treba da se vratimo: IDLE ili PAN_IDLE
    if (m_state == GraphView::State::PAN_IDLE || event->button() == Qt::RightButton) {
        // ako je kliknut desni klik da se odradi ispravan reset cursora
        m_oldState = m_state;
        // pan init
        setState(GraphView::State::PAN_ACTIVE);
        m_lastMousePos = event->pos();

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
        // kada se ispusti desni klik ne ostaje se u pan modu, to je samo precica za brzi pan
        if (event->button() == Qt::RightButton) {
            setState(m_oldState);
        } else {
            setState(GraphView::State::PAN_IDLE);
        }

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

void GraphView::wheelEvent(QWheelEvent* event) {
    if(event->angleDelta().y() > 0) {
        zoomIn();
    } else {
        zoomOut();
    }
}

void GraphView::init() {
    setRenderHint(QPainter::Antialiasing);
    setAlignment(Qt::AlignCenter);
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    setResizeAnchor(QGraphicsView::AnchorViewCenter);
}
