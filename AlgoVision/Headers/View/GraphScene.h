#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <QGraphicsScene>


class GraphScene : public QGraphicsScene {
public:
    explicit GraphScene(QObject *parent = nullptr);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    void addNode(QPointF position);
};

#endif // GRAPHSCENE_H
