#include "GraphView.h"

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


void GraphView::mousePressEvent(QMouseEvent* event) {
    if(m_state == GraphView::State::IDLE) {
        return;
    }
}

void GraphView::mouseMoveEvent(QMouseEvent* event) {
    if(m_state == GraphView::State::IDLE) {
        return;
    }
}

void GraphView::mouseReleaseEvent(QMouseEvent* event) {
    if(m_state == GraphView::State::IDLE) {
        return;
    }
}

void GraphView::init() {
    setAlignment(Qt::AlignCenter);
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    setResizeAnchor(QGraphicsView::AnchorViewCenter);
}
