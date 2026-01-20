#ifndef EDITABLE_TEXTITEM_H
#define EDITABLE_TEXTITEM_H

#include <QGraphicsTextItem>

class EditableTextItem : public QGraphicsTextItem {
    Q_OBJECT

public:
    explicit EditableTextItem(QGraphicsItem* parent = nullptr);

    QString oldText() const;
    void    setOldText(const QString& newOldText);

    void startEditing();
    void finishEditing(bool commit);
    void centerText();

    QPointF center() const;
    void setCenter(const QPointF& newCenter);
    void setCenter(qreal x, qreal y);

protected:
    void keyPressEvent(QKeyEvent* event) override;

signals:
    void textCommited(const QString& text);

    // notify graphscene if it's EDIT state or not (then it's ADD state)
    void setEditGraphSceneState(bool edit, EditableTextItem* label = nullptr);

private:
    QString m_oldText;
    QPointF m_center;
};

#endif
