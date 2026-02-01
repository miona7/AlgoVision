#ifndef GRAPHEDITTAB_H
#define GRAPHEDITTAB_H

#include <QWidget>

class QPushButton;
class QSlider;

class GraphEditTab : public QWidget {
    Q_OBJECT

public:
    explicit GraphEditTab(QWidget* = nullptr);
    ~GraphEditTab() override = default;

signals:
    void undoRequested();
    void redoRequested();
    void addRequested();
    void removeRequested();
    void clearRequested();

    void panRequested();
    void zoomInRequested();
    void zoomOutRequested();

public slots:
    void setUndoEnabled(bool);
    void setRedoEnabled(bool);

private:
    QPushButton* m_addBtn;
    QPushButton* m_removeBtn;
    QPushButton* m_undoBtn;
    QPushButton* m_redoBtn;
    QPushButton* m_zoomInBtn;
    QPushButton* m_zoomOutBtn;
    QPushButton* m_panBtn;
    QPushButton* m_clearBtn;

    QSlider* m_nodeSizeSlider;

    void initLayout();
};

#endif // GRAPHEDITTAB_H
