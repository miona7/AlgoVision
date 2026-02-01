#ifndef GRAPH_VIEW_H
#define GRAPH_VIEW_H

#include <QGraphicsView>


class GraphView : public QGraphicsView {
public:
    explicit GraphView(QWidget* = nullptr);
    explicit GraphView(QGraphicsScene*, QWidget* = nullptr);

private:
    // inicijalna podesavanja
    void init();
};

#endif // GRAPH_VIEW
