#include <QFile>
#include <QFileDialog>
#include <QJsonDocument>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QVariantMap>
#include <QVBoxLayout>

#include <iostream>

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Graph.h"
#include "GraphEditor.h"
#include "Serializer.h"
#include "UnweightedDirectedGraph.h"
#include "UnweightedUndirectedGraph.h"
#include "WeightedDirectedGraph.h"
#include "WeightedUndirectedGraph.h"


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), m_ui(new Ui::MainWindow), m_themeManager(new ThemeManager()) {
    m_ui->setupUi(this);

    m_serializer = std::make_unique<Serializer>();
    m_graph      = createGraph(false, false); // default unweighted, undirected

    this->setStyleSheet(m_themeManager->styleSheet());

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

    // ===== GRAPH PAGE CONTENT =====
    QWidget* graphPage = m_ui->graphPage;

    // layout
    if(graphPage->layout() == nullptr) {
        auto* graphLayout = new QVBoxLayout(graphPage);
        graphLayout->setContentsMargins(0, 0, 0, 0);

        auto* graphEditor = new GraphEditor(graphPage);
        m_graphEditor     = graphEditor; // mora zato sto je graphEditor lokalna promenljiva, necemo
                                         // imati pristup kasnije (cim konstruktor zavrsi, brise se)
        graphLayout->addWidget(graphEditor);
    }
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

    if(m_serializer == nullptr) {
        QMessageBox::warning(this, "error", "serializer is not initialized");
        return;
    }

    // pre-read isWeighted/isDirected iz json root-a
    QFile file(filePath);
    if(!file.open(QFile::ReadOnly)) {
        QMessageBox::warning(this, "error", "could not open file: " + filePath);
        return;
    }
    const auto jsonDoc = QJsonDocument::fromJson(file.readAll());
    file.close();
    const QVariantMap root       = jsonDoc.toVariant().toMap();
    const bool        isWeighted = root.value("isWeighted").toBool();
    const bool        isDirected = root.value("isDirected").toBool();

    m_graph = createGraph(isWeighted, isDirected);

    bool loadedWeighted = false;
    bool loadedDirected = false;
    m_serializer->load(*m_graph, filePath, loadedWeighted, loadedDirected);

    m_ui->stackedWidget->setCurrentWidget(m_ui->graphPage);
    std::cout << "btnOpenGraph clicked: "
              << m_ui->stackedWidget->currentWidget()->objectName().toStdString() << std::endl;

    initMenuToolBar();

    QMessageBox::information(this, "graph opened",
                             "loaded file: " + filePath +
                                 "\nweighted: " + QString(loadedWeighted ? "true" : "false") +
                                 "\ndirected: " + QString(loadedDirected ? "true" : "false"));

    // TODO: kada napravimo GraphEditor API:
    // if(m_graphEditor) m_graphEditor->setGraph(m_graph);
}

void MainWindow::onCreateGraphTriggered() {
    m_ui->stackedWidget->setCurrentWidget(m_ui->graphPage);
    this->setWindowTitle(QString::fromLatin1(AppConstants::graphPageDefaultTitle));
    std::cout << "btnCreateGraph clicked: "
              << m_ui->stackedWidget->currentWidget()->objectName().toStdString() << std::endl;

    initMenuToolBar();
}

void MainWindow::onSaveGraphTriggered() {
    QString filePath = QFileDialog::getSaveFileName(this, "save graph", "", "graph files (*.json)");

    if(filePath.isEmpty()) {
        return;
    }

    if(!filePath.endsWith(".json")) {
        filePath += ".json";
    }

    if(m_graph == nullptr || m_serializer == nullptr) {
        QMessageBox::warning(this, "error", "graph/serializer is not initialized");
        return;
    }

    // kreiranje fajla
    QFile file(filePath);
    if(file.open(QIODevice::WriteOnly)) {
        file.close();
        m_serializer->save(*m_graph, filePath, m_graph->isWeighted(), m_graph->isDirected());
        QMessageBox::information(this, "saved", "graph saved to: " + filePath);
    } else {
        QMessageBox::warning(this, "error", "could not save file: " + filePath);
    }
}

void MainWindow::onSaveImageTriggered() {
    QString filePath = QFileDialog::getSaveFileName(this, "save image", "", "image files (*.png)");

    if(filePath.isEmpty()) {
        return;
    }

    if(!filePath.endsWith(".png")) {
        filePath += ".png";
    }

    // TODO: cuvati samo scenu grafa, ne ceo widget

    QPixmap pixmap = m_ui->graphPage->grab();

    // cuvamo pix mapu u fajl
    if(pixmap.save(filePath, "PNG")) {
        QMessageBox::information(this, "saved", "image saved to: " + filePath);
    } else {
        QMessageBox::warning(this, "error", "could not save image: " + filePath);
    }
}

void MainWindow::onChangeThemeTriggered() {
    // rotira teme: DARK -> LIGHT -> PURPLE -> DARK
    switch(m_themeManager->currentTheme()) {
    case ThemeManager::Theme::DARK:
        m_themeManager->setTheme(ThemeManager::Theme::LIGHT);
        break;
    case ThemeManager::Theme::LIGHT:
        m_themeManager->setTheme(ThemeManager::Theme::PURPLE);
        break;
    case ThemeManager::Theme::PURPLE:
        m_themeManager->setTheme(ThemeManager::Theme::DARK);
        break;
    }

    this->setStyleSheet(m_themeManager->styleSheet());
}

void MainWindow::initMenuToolBar() {
    // dodajemo menu tool bar samo 1, akcije povezujemo samo 1

    if(m_menuToolBar != nullptr) {
        return;
    }

    m_menuToolBar = new MenuToolBar(this);
    m_menuToolBar->setMovable(false);
    addToolBar(Qt::TopToolBarArea, m_menuToolBar);

    connect(m_menuToolBar->openGraphAction(), &QAction::triggered, this,
            &MainWindow::onOpenGraphTriggered);
    connect(m_menuToolBar->createGraphAction(), &QAction::triggered, this,
            &MainWindow::onCreateGraphTriggered);
    connect(m_menuToolBar->saveGraphAction(), &QAction::triggered, this,
            &MainWindow::onSaveGraphTriggered);
    connect(m_menuToolBar->saveImageAction(), &QAction::triggered, this,
            &MainWindow::onSaveImageTriggered);
    connect(m_menuToolBar->changeThemeAction(), &QAction::triggered, this,
            &MainWindow::onChangeThemeTriggered);
}

std::shared_ptr<Graph> MainWindow::createGraph(bool isWeighted, bool isDirected) {
    if(isWeighted && isDirected) {
        return std::make_shared<WeightedDirectedGraph>();
    }
    if(isWeighted && !isDirected) {
        return std::make_shared<WeightedUndirectedGraph>();
    }
    if(!isWeighted && isDirected) {
        return std::make_shared<UnweightedDirectedGraph>();
    }
    return std::make_shared<UnweightedUndirectedGraph>();
}
