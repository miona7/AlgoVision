#pragma once

#include <QWidget>

class AlgorithmTab : public QWidget
{
    Q_OBJECT

public:
    explicit AlgorithmTab(QWidget* parent = nullptr);
    ~AlgorithmTab() override = default;
};
