#ifndef NODEITEM_H
#define NODEITEM_H

#include <QPen>
#include <QSet>
#include <QPainter>
#include <QGraphicsObject>
#include <QGraphicsSceneEvent>
#include <QGraphicsSceneMouseEvent>

#include "Node.h"
#include "EdgeItem.h"
#include "AppConstants.h"
#include "EditableTextItem.h"

class EdgeItem;

class NodeItem : public QGraphicsObject {
    Q_OBJECT
public:
    explicit NodeItem(Node*);
    ~NodeItem() override;

    Node* modelNode() const;
    void  setModelNode(Node*);
    qreal radius() const;

    void setNodeSelected(bool);
    void updateSize();

    void addEdge(EdgeItem*);
    void removeEdge(EdgeItem*);

    void updateNodePosition();

    EditableTextItem* label() const;
    void              setLabel(EditableTextItem*);

    const QSet<EdgeItem*>& edges() const;

signals:
    void nodeSelected(NodeItem*);
    void editNodeNameRequest(const NodeItem*, const QString&) const;
    void moveNodeRequest(const NodeItem*, const QPointF&, const QPointF&);

protected:
    QRectF       boundingRect() const override;
    QPainterPath shape() const override;
    void         paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;
    QVariant     itemChange(GraphicsItemChange, const QVariant&) override;
    void         mousePressEvent(QGraphicsSceneMouseEvent*) override;
    void         mouseMoveEvent(QGraphicsSceneMouseEvent*) override;
    void         mouseReleaseEvent(QGraphicsSceneMouseEvent*) override;

    // removed feature
    // it will remain in code all connected, so the feature can easily be activated again, or used
    // for other purposes
    // void         mouseDoubleClickEvent(QGraphicsSceneMouseEvent*) override;

private slots:
    void onNameChanged(const QString&) const;

private:
    Node*             m_modelNode{nullptr};
    EditableTextItem* m_label{nullptr};
    QSet<EdgeItem*>   m_edges;
    // in scene coordinates, used to save old node position(center) before move command
    QPointF m_oldCenter;

    qreal m_radius{AppConstants::defaultRadius};
    qreal m_borderWidth{AppConstants::defaultBorderWidth};
    bool  m_hasChangePosition{false};
    bool  m_nodeSelected{false};
    bool  m_ignoreNextMouseRealese{false};

    unsigned m_observerId{0};

    QColor calculateColor() const;
    void   onNodeUpdated();
};

#endif // NODEITEM_H
