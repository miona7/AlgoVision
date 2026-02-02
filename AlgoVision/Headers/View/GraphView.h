#ifndef GRAPH_VIEW_H
#define GRAPH_VIEW_H

#include <QGraphicsView>


class GraphView : public QGraphicsView {
public:
    enum class State {IDLE, PAN_IDLE, PAN_ACTIVE, ZOOM_IN, ZOOM_OUT};

    explicit GraphView(QWidget* = nullptr);
    explicit GraphView(QGraphicsScene*, QWidget* = nullptr);

    GraphView::State state() const;
    void setState(GraphView::State);
    void resetState();

protected:
    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    GraphView::State m_state {GraphView::State::IDLE};

    // inicijalna podesavanja
    void init();

    // pan state (u koordinatama scene)
    QPoint m_lastMousePos;
};

#endif // GRAPH_VIEW
