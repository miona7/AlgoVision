#include "GraphEditor.h"

#include <QHBoxLayout>
#include <QTabWidget>
#include <QSplitter>
#include <QLabel>

#include "GraphEditTab.h"
#include "AlgorithmTab.h"

GraphEditor::GraphEditor(QWidget* parent) : QWidget(parent){

    // main splitter for the left and right page sides
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);

    // left side (placeholder)
    QLabel* leftPlaceholder = new QLabel("GRAPH / SCENE AREA", splitter);
    leftPlaceholder->setAlignment(Qt::AlignCenter);
    leftPlaceholder->setStyleSheet("background-color: #2b2b2b; color: white;");

    splitter->addWidget(leftPlaceholder);

    // right side
    QTabWidget* rightTabs = new QTabWidget(splitter);
    rightTabs->addTab(new GraphEditTab(rightTabs), "graph edit");
    rightTabs->addTab(new AlgorithmTab(rightTabs), "algorithm");

    splitter->addWidget(rightTabs);

    // 80% left, 20% right
    splitter->setStretchFactor(0, 8);
    splitter->setStretchFactor(1, 2);

    // layout for the whole GraphEditor
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(splitter);
}
