#ifndef GRAPH_VIEW_H
#define GRAPH_VIEW_H

#include <QGraphicsView>


class GraphView : public QGraphicsView {
public:
    enum class State {
        IDLE
    };

    explicit GraphView(QWidget* = nullptr);
    explicit GraphView(QGraphicsScene*, QWidget* = nullptr);

    GraphView::State state() const;
    void setState(GraphView::State);

protected:
    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;

private:
    GraphView::State m_state {GraphView::State::IDLE};

    // inicijalna podesavanja
    void init();
};

#endif // GRAPH_VIEW
