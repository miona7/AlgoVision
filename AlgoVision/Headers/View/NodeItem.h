#ifndef NODEITEM_H
#define NODEITEM_H

#include <AppConstants.h>
#include <Node.h>
#include <QGraphicsObject>

class EdgeItem;
class EditableTextItem;

class NodeItem : public QGraphicsObject {
    Q_OBJECT

public:
    explicit NodeItem(Node*);
    ~NodeItem();

    Node* modelNode() const;
    void  setModelNode(Node*);
    qreal radius() const;
    void  setRadius(qreal);
    void  setNodeSelected(bool);

    void addEdge(EdgeItem*);
    void removeEdge(EdgeItem*);

    EditableTextItem* label() const;
    void              setLabel(EditableTextItem* newLabel);

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
    void         mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;

private slots:
    void onNameChanged(const QString& name) const;

private:
    Node*             m_modelNode;
    EditableTextItem* m_label;
    QSet<EdgeItem*>   m_edges;

    qreal m_radius {AppConstants::defaultRadius};
    qreal m_borderWidth {AppConstants::defaultBorderWidth};
    bool  m_hasChangePosition {false};
    bool  m_nodeSelected {false};
    bool  m_ignoreNextMouseRealese {false};

    unsigned m_observerId {0};

    const QColor calculateColor() const;
    void         onNodeUpdated();
};

#endif // NODEITEM_H
