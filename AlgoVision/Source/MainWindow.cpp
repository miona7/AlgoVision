#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <iostream>
#include <QPushButton>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), m_ui(new Ui::MainWindow) {
    m_ui->setupUi(this);


    QWidget* startPage = m_ui->startPage;
    std::cout << "current page: " << m_ui->stackedWidget->currentWidget()->objectName().toStdString() << std::endl;

    auto* startLayout = new QHBoxLayout(startPage);
    startLayout->setAlignment(Qt::AlignCenter);
    startLayout->setSpacing(24);

    auto* btnOpenGraph = new QPushButton("open graph", startPage);
    btnOpenGraph->setObjectName("newOpenGraph");
    btnOpenGraph->setFixedSize(QSize(100, 100));
    startLayout->addWidget(btnOpenGraph);

    auto* btnCreateGraph = new QPushButton("create graph", startPage);
    btnCreateGraph->setObjectName("newCreateGraph");
    btnCreateGraph->setFixedSize(QSize(100, 100));
    startLayout->addWidget(btnCreateGraph);

    connect(btnOpenGraph, &QPushButton::clicked, this, &MainWindow::onOpenGraphTriggered);
    connect(btnCreateGraph, &QPushButton::clicked, this, &MainWindow::onCreateGraphTriggered);
}

MainWindow::~MainWindow() {
    delete m_ui;
}

void MainWindow::onOpenGraphTriggered()
{
    m_ui->stackedWidget->setCurrentWidget(m_ui->graphPage);
    std::cout << "btnOpenGraph clicked: " << m_ui->stackedWidget->currentWidget()->objectName().toStdString() << std::endl;
    /*TODO: logic*/
}

void MainWindow::onCreateGraphTriggered()
{
    m_ui->stackedWidget->setCurrentWidget(m_ui->graphPage);
    std::cout << "btnCreateGraph clicked: " << m_ui->stackedWidget->currentWidget()->objectName().toStdString() << std::endl;
    /*TODO: logic*/
}
