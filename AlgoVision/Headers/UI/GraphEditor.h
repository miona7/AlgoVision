#ifndef GRAPHEDITOR_H
#define GRAPHEDITOR_H

#include <QWidget>

class QUndoStack;
class GraphEditTab;
class QLabel;

class GraphEditor : public QWidget {
    Q_OBJECT

public:
    explicit GraphEditor(QWidget* parent = nullptr);
    ~GraphEditor() override = default;

private:
    QUndoStack*   m_undoStack       = nullptr;
    GraphEditTab* m_editTab         = nullptr;
    QLabel*       m_leftPlaceholder = nullptr;

    int m_dummyState = 0; // samo za test
};

#endif // GRAPHEDITOR_H
