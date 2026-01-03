#include <iostream>

#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QString>

#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), m_ui(new Ui::MainWindow) {
    m_ui->setupUi(this);

    this->setWindowTitle(QString::fromLatin1(AppConstants::startPageTitle));
    this->setMinimumWidth(AppConstants::windowMinWidth);
    this->setMinimumHeight(AppConstants::windowMinHeight);
    this->resize(QSize(AppConstants::windowWidth, AppConstants::windowHeight));

    QWidget* startPage = m_ui->startPage;
    std::cout << "current page: "
              << m_ui->stackedWidget->currentWidget()->objectName().toStdString() << std::endl;

    auto* startLayout = new QHBoxLayout(startPage);
    startLayout->setAlignment(Qt::AlignCenter);
    startLayout->setSpacing(AppConstants::btnSpacing);

    auto* btnOpenGraph = new QPushButton("open graph", startPage);
    btnOpenGraph->setObjectName("newOpenGraph");
    btnOpenGraph->setFixedSize(QSize(AppConstants::btnSize, AppConstants::btnSize));
    startLayout->addWidget(btnOpenGraph);

    auto* btnCreateGraph = new QPushButton("create graph", startPage);
    btnCreateGraph->setObjectName("newCreateGraph");
    btnCreateGraph->setFixedSize(QSize(AppConstants::btnSize, AppConstants::btnSize));
    startLayout->addWidget(btnCreateGraph);

    connect(btnOpenGraph, &QPushButton::clicked, this, &MainWindow::onOpenGraphTriggered);
    connect(btnCreateGraph, &QPushButton::clicked, this, &MainWindow::onCreateGraphTriggered);
}

MainWindow::~MainWindow() {
    delete m_ui;
}

void MainWindow::onOpenGraphTriggered() {

    QString filePath = QFileDialog::getOpenFileName(this, "open graph", "", "graph files (*.json)");

    // cancel -> vracamo se
    if(filePath.isEmpty()) {
        return;
    }

    QMessageBox::information(this, "graph opened", "selected file: " + filePath);

    m_ui->stackedWidget->setCurrentWidget(m_ui->graphPage);
    std::cout << "btnOpenGraph clicked: "
              << m_ui->stackedWidget->currentWidget()->objectName().toStdString() << std::endl;

    initMenuToolBar();
}

void MainWindow::onCreateGraphTriggered() {
    m_ui->stackedWidget->setCurrentWidget(m_ui->graphPage);
    this->setWindowTitle(QString::fromLatin1(AppConstants::graphPageDefaultTitle));
    std::cout << "btnCreateGraph clicked: "
              << m_ui->stackedWidget->currentWidget()->objectName().toStdString() << std::endl;

    initMenuToolBar();
}

void MainWindow::initMenuToolBar() {
    // dodajemo menu tool bar samo 1, akcije povezujemo samo 1

    if(m_menuToolBar != nullptr) {
        return;
    }

    m_menuToolBar = new MenuToolBar(this);
    m_menuToolBar->setMovable(false);
    addToolBar(Qt::TopToolBarArea, m_menuToolBar);

    connect(m_menuToolBar->openGraphAction(), &QAction::triggered, this, &MainWindow::onOpenGraphTriggered);
    connect(m_menuToolBar->createGraphAction(), &QAction::triggered, this, &MainWindow::onCreateGraphTriggered);
}
