#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>

#include "GraphEditTab.h"

GraphEditTab::GraphEditTab(QWidget* parent)
    : QWidget(parent), m_addBtn(new QPushButton("add", this)),
      m_removeBtn(new QPushButton("remove", this)), m_undoBtn(new QPushButton("undo", this)),
      m_redoBtn(new QPushButton("redo", this)), m_zoomInBtn(new QPushButton("zoom in", this)),
      m_zoomOutBtn(new QPushButton("zoom out", this)), m_panBtn(new QPushButton("pan", this)),
      m_clearBtn(new QPushButton("clear", this)),
      m_nodeSizeSlider(new QSlider(Qt::Horizontal, this)) {
    initLayout();

    // emitujemo signale (GraphEditor se onda pita)
    connect(m_undoBtn, &QPushButton::clicked, this, &GraphEditTab::undoRequested);
    connect(m_redoBtn, &QPushButton::clicked, this, &GraphEditTab::redoRequested);
    connect(m_addBtn, &QPushButton::clicked, this, &GraphEditTab::addRequested);
    connect(m_removeBtn, &QPushButton::clicked, this, &GraphEditTab::removeRequested);
    connect(m_clearBtn, &QPushButton::clicked, this, &GraphEditTab::clearRequested);
    connect(m_panBtn, &QPushButton::clicked, this, &GraphEditTab::panRequested);
    connect(m_zoomInBtn, &QPushButton::clicked, this, &GraphEditTab::zoomInRequested);
    connect(m_zoomOutBtn, &QPushButton::clicked, this, &GraphEditTab::zoomOutRequested);

    // default stanje
    m_undoBtn->setEnabled(false);
    m_redoBtn->setEnabled(false);
}

void GraphEditTab::setUndoEnabled(bool enabled) {
    m_undoBtn->setEnabled(enabled);
}

void GraphEditTab::setRedoEnabled(bool enabled) {
    m_redoBtn->setEnabled(enabled);
}

void GraphEditTab::initLayout() {
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(8);

    // edit
    auto* graphEditBox    = new QGroupBox("edit graph", this);
    auto* graphEditLayout = new QGridLayout(graphEditBox);
    graphEditLayout->setSpacing(6);

    graphEditLayout->addWidget(m_addBtn, 0, 0);
    graphEditLayout->addWidget(m_removeBtn, 0, 1);

    mainLayout->addWidget(graphEditBox);

    // history
    auto* historyBox    = new QGroupBox("history", this);
    auto* historyLayout = new QGridLayout(historyBox);
    historyLayout->setSpacing(6);

    historyLayout->addWidget(m_undoBtn, 0, 0);
    historyLayout->addWidget(m_redoBtn, 0, 1);

    mainLayout->addWidget(historyBox);

    // view
    auto* viewBox    = new QGroupBox("view", this);
    auto* viewLayout = new QGridLayout(viewBox);
    viewLayout->setSpacing(6);

    viewLayout->addWidget(m_zoomInBtn, 0, 0);
    viewLayout->addWidget(m_zoomOutBtn, 0, 1);

    mainLayout->addWidget(viewBox);

    // scene
    auto* sceneBox    = new QGroupBox("scene", this);
    auto* sceneLayout = new QGridLayout(sceneBox);
    sceneLayout->setSpacing(6);

    sceneLayout->addWidget(m_clearBtn, 0, 0);
    sceneLayout->addWidget(m_panBtn, 0, 1);

    mainLayout->addWidget(sceneBox);

    // node settings
    auto* nodeBox    = new QGroupBox("node settings", this);
    auto* nodeLayout = new QVBoxLayout(nodeBox);
    nodeLayout->setSpacing(6);

    auto* sliderLabel = new QLabel("size", this);
    nodeLayout->addWidget(sliderLabel);
    nodeLayout->addWidget(m_nodeSizeSlider);

    mainLayout->addWidget(nodeBox);

    mainLayout->addStretch();
}

QSlider* GraphEditTab::getNodeSizeSlider() const {
    return m_nodeSizeSlider;
}
