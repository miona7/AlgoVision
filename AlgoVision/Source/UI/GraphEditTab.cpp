#include "GraphEditTab.h"

#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>

GraphEditTab::GraphEditTab(QWidget* parent)
    : QWidget(parent),
      m_addBtn(new QPushButton("add", this)),
      m_removeBtn(new QPushButton("remove", this)),
      m_undoBtn(new QPushButton("undo", this)),
      m_redoBtn(new QPushButton("redo", this)),
      m_zoomInBtn(new QPushButton("zoom in", this)),
      m_zoomOutBtn(new QPushButton("zoom out", this)),
      m_panBtn(new QPushButton("pan", this)),
      m_clearBtn(new QPushButton("clear", this)),
      m_nodeSizeSlider(new QSlider(Qt::Horizontal, this))
{
    initLayout();
}

void GraphEditTab::initLayout(){
    auto* mainLayout = new QVBoxLayout(this);

    auto* buttonLayout = new QGridLayout();
    buttonLayout->setSpacing(6);

    //buttons
    buttonLayout->addWidget(m_addBtn,     0, 0);
    buttonLayout->addWidget(m_removeBtn,  0, 1);

    buttonLayout->addWidget(m_undoBtn,    1, 0);
    buttonLayout->addWidget(m_redoBtn,    1, 1);

    buttonLayout->addWidget(m_zoomInBtn,  2, 0);
    buttonLayout->addWidget(m_zoomOutBtn, 2, 1);

    buttonLayout->addWidget(m_panBtn,     3, 0);
    buttonLayout->addWidget(m_clearBtn,   3, 1);

    mainLayout->addLayout(buttonLayout);

    //slider
    auto* sliderLabel = new QLabel("node size", this);
    mainLayout->addWidget(sliderLabel);
    mainLayout->addWidget(m_nodeSizeSlider);

    mainLayout->addStretch();
}
