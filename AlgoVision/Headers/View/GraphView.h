#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include <QScrollBar>
#include <QGraphicsView>

#include <qevent.h>

class GraphView : public QGraphicsView {
public:
    enum class State { IDLE, PAN_IDLE, PAN_ACTIVE };

    explicit GraphView(QWidget* = nullptr);
    explicit GraphView(QGraphicsScene*, QWidget* = nullptr);

    GraphView::State state() const;
    void             setState(GraphView::State);
    void             resetState();

    void zoomIn();
    void zoomOut();

protected:
    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    GraphView::State m_state{GraphView::State::IDLE};

    // pan state (in scene coordinates)
    QPoint m_lastMousePos;

    qreal m_zoomFactor{1.15};

    // used for returning to previous state after fast pan (right click drag)
    GraphView::State m_oldState;

    void init();
};

#endif // GRAPHVIEW_H
