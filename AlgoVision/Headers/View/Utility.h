#ifndef UTILITY_H
#define UTILITY_H

#include <QPointF>
#include <qnumeric.h>
#include <qtypes.h>


namespace Utility {
    bool qrealEqual(qreal a, qreal b) {
        constexpr qreal eps = 1e-6;
        return qAbs(a - b) <= eps;
    }

    bool qrealLess(qreal a, qreal b) {
        return a < b && !qrealEqual(a, b);
    }

    bool qrealGreater(qreal a, qreal b) {
        return a > b && !qrealEqual(a, b);
    }

    int lineOrientation(QPointF p1, QPointF p2) {
        int orientation;
        if (qrealLess(p1.x(), p2.x()) || (qrealEqual(p1.x(), p2.x()) && qrealLess(p1.y(), p2.y()))) {
            orientation = 1;
        } else {
            orientation = -1;
        }

        return orientation;
    }
}

#endif // UTILITY
