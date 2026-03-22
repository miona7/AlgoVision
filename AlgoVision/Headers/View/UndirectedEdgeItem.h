#ifndef UNDIRECTEDEDGEITEM_H
#define UNDIRECTEDEDGEITEM_H

#include <QPainter>
#include <QPen>

#include "AppConstants.h"
#include "EdgeItem.h"

class UndirectedEdgeItem : public EdgeItem {
public:
    UndirectedEdgeItem(Edge*, NodeItem*, NodeItem*, bool = false);

    QRectF       boundingRect() const override;
    QPainterPath shape() const override;
    void         paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;
    QPainterPath edgePath() const override;
};

#endif // UNDIRECTEDEDGEITEM_H
