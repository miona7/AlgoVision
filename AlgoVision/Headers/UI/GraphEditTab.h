#ifndef GRAPHEDITTAB_H
#define GRAPHEDITTAB_H

#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QWidget>

class GraphEditTab : public QWidget {
    Q_OBJECT

public:
    explicit GraphEditTab(QWidget* = nullptr);
    ~GraphEditTab() override = default;
    QSlider* getNodeSizeSlider() const;

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
    void setGraphEditsAllowed(bool);

private:
    QPushButton* m_addBtn;
    QPushButton* m_removeBtn;
    QPushButton* m_undoBtn;
    QPushButton* m_redoBtn;
    QPushButton* m_zoomInBtn;
    QPushButton* m_zoomOutBtn;
    QPushButton* m_panBtn;
    QPushButton* m_clearBtn;
    QSlider*     m_nodeSizeSlider;

    bool m_graphEditsAllowed = true;
    bool m_canUndo           = false;
    bool m_canRedo           = false;

    void applyEditButtonsState();
    void initLayout();
};

#endif // GRAPHEDITTAB_H
