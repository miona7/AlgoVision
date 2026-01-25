#ifndef EDITABLE_TEXTITEM_H
#define EDITABLE_TEXTITEM_H

#include <QGraphicsTextItem>

class EditableTextItem : public QGraphicsTextItem {
    Q_OBJECT

public:
    explicit EditableTextItem(QGraphicsItem* = nullptr);

    QString oldText() const;
    void    setOldText(const QString&);

    void startEditing();
    void finishEditing(bool);
    void centerText();

    QPointF center() const;
    void    setCenter(const QPointF&);
    void    setCenter(qreal, qreal);

protected:
    void keyPressEvent(QKeyEvent*) override;

signals:
    void textCommited(const QString& text);

    // notify graphscene if it's EDIT state or not (then it's ADD state)
    void setEditGraphSceneState(bool, EditableTextItem* = nullptr);

private:
    QString m_oldText;
    QPointF m_center;
};

#endif
