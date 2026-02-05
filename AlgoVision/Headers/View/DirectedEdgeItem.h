#ifndef DIRECTED_EDGEITEM_H
#define DIRECTED_EDGEITEM_H

#include <QPainter>
#include <QPen>
#include <cmath>

#include "EdgeItem.h"
#include "AppConstants.h"

class DirectedEdgeItem : public EdgeItem {

public:
    DirectedEdgeItem(Edge*, NodeItem*, NodeItem*, bool = false);

    QRectF       boundingRect() const override;
    QPainterPath shape() const override;
    void         paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;

    QPainterPath edgePath() const override;

private:
    qreal m_skewness {70.0};
    qreal m_arrowSize {20.0};
    qreal m_arrowAngle {M_PI / 6.0};

    QPainterPath arrowPath(const QPainterPath&) const;
};

#endif // DIRECTED_EDGEITEM_H
