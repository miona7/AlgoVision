#include "EditableTextItem.h"

EditableTextItem::EditableTextItem(QGraphicsItem* parent) : QGraphicsTextItem(parent) {
    QTextOption opt;
    opt.setAlignment(Qt::AlignCenter);
    opt.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    document()->setDefaultTextOption(opt);
}

QString EditableTextItem::oldText() const {
    return m_oldText;
}

void EditableTextItem::setOldText(const QString& newOldText) {
    m_oldText = newOldText;
}

void EditableTextItem::startEditing() {
    m_oldText = toPlainText();
    setTextInteractionFlags(Qt::TextEditorInteraction);
    setFocus();

    QTextCursor c = textCursor();
    c.select(QTextCursor::Document);
    setTextCursor(c);

    emit setEditGraphSceneState(true, this);
}

void EditableTextItem::finishEditing(bool commit) {
    QTextCursor c = textCursor();
    c.clearSelection();
    setTextCursor(c);

    setTextInteractionFlags(Qt::NoTextInteraction);
    clearFocus();

    if(!commit) {
        setPlainText(m_oldText);
    } else {
        emit textCommited(toPlainText());
    }

    emit setEditGraphSceneState(false);
    centerText();
}

void EditableTextItem::centerText() {
    QRectF r = boundingRect();
    setPos(m_center.x() - (r.width() / 2), m_center.y() - (r.height() / 2));
}

QPointF EditableTextItem::center() const {
    return m_center;
}

void EditableTextItem::setCenter(const QPointF& newCenter) {
    m_center = newCenter;
}

void EditableTextItem::setCenter(qreal x, qreal y) {
    m_center.setX(x);
    m_center.setY(y);
}

void EditableTextItem::keyPressEvent(QKeyEvent* event) {
    if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        finishEditing(true);
        return;
    }

    if(event->key() == Qt::Key_Escape) {
        finishEditing(false);
        return;
    }

    QGraphicsTextItem::keyPressEvent(event);
}
