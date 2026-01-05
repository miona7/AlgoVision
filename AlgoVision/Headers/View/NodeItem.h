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

protected:
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    Node* m_modelNode;
    qreal m_radius{AppConstants::defaultRadius};
    qreal m_borderWidth{AppConstants::defaultBorderWidth};
};

#endif // NODEITEM_H
