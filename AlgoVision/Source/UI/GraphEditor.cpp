#include "GraphEditor.h"

#include <QHBoxLayout>
#include <QTabWidget>
#include <QSplitter>
#include <QLabel>

#include "GraphEditTab.h"
#include "AlgorithmTab.h"

GraphEditor::GraphEditor(QWidget* parent)
    : QWidget(parent)
{
    // GLAVNI SPLITTER: levo sadrzaj, desno panel
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);

           // ===== LEVA STRANA (placeholder za scenu / graf) =====
    QLabel* leftPlaceholder = new QLabel("GRAPH / SCENE AREA", splitter);
    leftPlaceholder->setAlignment(Qt::AlignCenter);
    leftPlaceholder->setStyleSheet("background-color: #2b2b2b; color: white;");

    splitter->addWidget(leftPlaceholder);

           // ===== DESNA STRANA (tvoj deo) =====
    QTabWidget* rightTabs = new QTabWidget(splitter);
    rightTabs->addTab(new GraphEditTab(rightTabs), "graph edit");
    rightTabs->addTab(new AlgorithmTab(rightTabs), "algorithm");

    splitter->addWidget(rightTabs);

           // 70% levo, 30% desno
    splitter->setStretchFactor(0, 7);
    splitter->setStretchFactor(1, 3);

           // layout za ceo GraphEditor
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(splitter);
    setLayout(layout);
}
