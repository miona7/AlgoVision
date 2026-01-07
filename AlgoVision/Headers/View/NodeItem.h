#ifndef NODEITEM_H
#define NODEITEM_H

#include <QGraphicsObject>
#include "Node.h"
#include "AppConstants.h"

class NodeItem : public QGraphicsObject {
    Q_OBJECT

public:
    explicit NodeItem(Node* model);

    Node* modelNode() const;
    void setModelNode(Node *newModel);

    qreal radius() const;
    void setRadius(qreal newRadius);

protected:
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    Node* m_modelNode;
    qreal m_radius{AppConstants::defaultRadius};
    qreal m_borderWidth{AppConstants::defaultBorderWidth};
    bool m_hasChangePosition{false};
    bool m_nodeSelected{false};

    const QColor calculateColor() const;
};

#endif // NODEITEM_H
