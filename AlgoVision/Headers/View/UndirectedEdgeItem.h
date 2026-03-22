#ifndef UNDIRECTEDEDGEITEM_H
#define UNDIRECTEDEDGEITEM_H

#include <QPen>
#include <QPainter>

#include "EdgeItem.h"
#include "AppConstants.h"

class UndirectedEdgeItem : public EdgeItem {
public:
    UndirectedEdgeItem(Edge*, NodeItem*, NodeItem*, bool = false);

    QRectF       boundingRect() const override;
    QPainterPath shape() const override;
    void         paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;
    QPainterPath edgePath() const override;
};

#endif // UNDIRECTEDEDGEITEM_H
