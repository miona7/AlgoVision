#ifndef NODEITEM_H
#define NODEITEM_H

#include <AppConstants.h>
#include <Node.h>
#include <QGraphicsObject>

class EdgeItem;

class NodeItem : public QGraphicsObject {
    Q_OBJECT

public:
    explicit NodeItem(Node*);
    ~NodeItem() override;

    Node* modelNode() const;
    void  setModelNode(Node*);
    qreal radius() const;
    void  setRadius(qreal);
    void  setNodeSelected(bool);

    void addEdge(EdgeItem*);
    void removeEdge(EdgeItem*);

signals:
    void nodeSelected(NodeItem*);

protected:
    QRectF       boundingRect() const override;
    QPainterPath shape() const override;
    void         paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;
    QVariant     itemChange(GraphicsItemChange, const QVariant&) override;
    void         mousePressEvent(QGraphicsSceneMouseEvent*) override;
    void         mouseMoveEvent(QGraphicsSceneMouseEvent*) override;
    void         mouseReleaseEvent(QGraphicsSceneMouseEvent*) override;

private:
    Node*           m_modelNode;
    QSet<EdgeItem*> m_edges;

    qreal m_radius {AppConstants::defaultRadius};
    qreal m_borderWidth {AppConstants::defaultBorderWidth};
    bool  m_hasChangePosition {false};
    bool  m_nodeSelected {false};

    unsigned m_observerId = 0;

    const QColor calculateColor() const;
    void onNodeUpdated();
};

#endif // NODEITEM_H
