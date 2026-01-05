#pragma once

#include <QWidget>

class GraphEditTab : public QWidget
{
    Q_OBJECT

public:
    explicit GraphEditTab(QWidget* parent = nullptr);
    ~GraphEditTab() override = default;
};
