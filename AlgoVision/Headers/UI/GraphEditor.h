#ifndef GRAPHEDITOR_H
#define GRAPHEDITOR_H

#include <QWidget>


class GraphEditor : public QWidget {
    Q_OBJECT

public:
    explicit GraphEditor(QWidget* parent = nullptr);
    ~GraphEditor() override = default;

private:
    //privatne clanice
};

#endif //GRAPHEDITOR_H
