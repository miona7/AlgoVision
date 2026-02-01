#include "GraphView.h"

GraphView::GraphView(QWidget* parent) : QGraphicsView(parent) {
}

GraphView::GraphView(QGraphicsScene* scene, QWidget* parent) : QGraphicsView(scene, parent) {
}
