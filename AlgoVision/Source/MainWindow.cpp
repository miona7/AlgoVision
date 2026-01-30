#include <QFile>
#include <QFileDialog>
#include <QJsonDocument>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>
#include <QVariantMap>
#include <QGroupBox>
#include <QRadioButton>
#include <QDialog>
#include <QDialogButtonBox>

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
}

MainWindow::~MainWindow() {
    delete m_ui;
}

void MainWindow::onOpenGraphTriggered() {
    QString filePath = QFileDialog::getOpenFileName(this, "open graph", "", "graph files (*.json)");

    if(filePath.isEmpty()) {
        return;
    }

    if(m_serializer == nullptr) {
        QMessageBox::warning(this, "error", "serializer is not initialized");
        return;
    }

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

    bool loadedWeighted = false;
    bool loadedDirected = false;

    m_ui->stackedWidget->setCurrentWidget(m_ui->graphPage);
    std::cout << "btnOpenGraph clicked: "
              << m_ui->stackedWidget->currentWidget()->objectName().toStdString() << std::endl;

    initMenuToolBar();

    QMessageBox::information(this, "graph opened",
                             "loaded file: " + filePath +
                                 "\nweighted: " + QString(loadedWeighted ? "true" : "false") +
                                 "\ndirected: " + QString(loadedDirected ? "true" : "false"));
}

void MainWindow::onCreateGraphTriggered() {

    QDialog dialog(this);
    dialog.setWindowTitle("Create Graph Options");
    dialog.setModal(true);
    dialog.setMinimumSize(400, 250);
    dialog.setSizeGripEnabled(true);
    dialog.setWindowFlags(dialog.windowFlags() | Qt::WindowMinMaxButtonsHint);


    QVBoxLayout* mainLayout = new QVBoxLayout(&dialog);

    QGroupBox* typeGroup = new QGroupBox("Graph Type", &dialog);
    QVBoxLayout* typeLayout = new QVBoxLayout(typeGroup);
    QRadioButton* directedBtn = new QRadioButton("Directed", typeGroup);
    QRadioButton* undirectedBtn = new QRadioButton("Undirected", typeGroup);
    undirectedBtn->setChecked(true);
    typeLayout->addWidget(directedBtn);
    typeLayout->addWidget(undirectedBtn);
    mainLayout->addWidget(typeGroup);

    QGroupBox* weightGroup = new QGroupBox("Weight", &dialog);
    QVBoxLayout* weightLayout = new QVBoxLayout(weightGroup);
    QRadioButton* weightedBtn = new QRadioButton("Weighted", weightGroup);
    QRadioButton* unweightedBtn = new QRadioButton("Unweighted", weightGroup);
    unweightedBtn->setChecked(true);
    weightLayout->addWidget(weightedBtn);
    weightLayout->addWidget(unweightedBtn);
    mainLayout->addWidget(weightGroup);

    QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                                     Qt::Horizontal, &dialog);
    mainLayout->addWidget(buttons);
    QObject::connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    bool directed = false;
    bool weighted = false;

    dialog.setStyleSheet(R"(
        QGroupBox {
            font-weight: bold;
            border: 1px solid #aaa;
            border-radius: 6px;
            margin-top: 10px;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 5px;
        }
        QRadioButton {
            padding: 4px;
        }
)");


    if(dialog.exec() == QDialog::Accepted) {
        directed = directedBtn->isChecked();
        weighted = weightedBtn->isChecked();

        std::cout << "Graph created with options: "
                  << (directed ? "Directed" : "Undirected") << ", "
                  << (weighted ? "Weighted" : "Unweighted") << std::endl;

    } else {

        return;
    }

    createGraphEditor(directed, weighted);

    m_ui->stackedWidget->setCurrentWidget(m_ui->graphPage);
    this->setWindowTitle(QString::fromLatin1(AppConstants::graphPageDefaultTitle));
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

    QFile file(filePath);
    if(file.open(QIODevice::WriteOnly)) {
        file.close();
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


    // cuvamo samo scenu
    QGraphicsView* view = m_graphEditor->graphController()->scene()->views().first();
    QPixmap pixmap = view->viewport()->grab();

    if(pixmap.save(filePath, "PNG")) {
        QMessageBox::information(this, "saved", "image saved to: " + filePath);
    } else {
        QMessageBox::warning(this, "error", "could not save image: " + filePath);
    }
}

void MainWindow::onChangeThemeTriggered() {
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

    connect(m_menuToolBar->helpAction(), &QAction::triggered, this, &MainWindow::onHelpTriggered);
}

void MainWindow::createGraphEditor(bool isDirected, bool isWeighted) {
    // prvo pravimo kontroler i graf(model + pogled)
    std::shared_ptr<GraphController> controller = std::make_shared<GraphController>();
    controller->createGraph(isDirected, isWeighted);

    // onda postavljamo graf editor(UI) koji je vezan za kontroler(model + pogled)
    QWidget* graphPage = m_ui->graphPage;

    if(graphPage->layout() == nullptr) {
        auto* graphLayout = new QVBoxLayout(graphPage);
        graphLayout->setContentsMargins(0, 0, 0, 0);

        auto* graphEditor = new GraphEditor(controller, graphPage);
        m_graphEditor     = graphEditor;
        graphLayout->addWidget(graphEditor);
    }
}

void MainWindow::onHelpTriggered() {
    QString helpText =
        "Graph Editor Help:\n\n"
        "- Use the right toolbar to add new elements.\n"
        "- Double-click on a node to create a new node.\n"
        "- To create a new edge, click on the two nodes you want to connect.\n"
        "- You can save the graph or export it as an image using the top toolbar.\n"
        "- For any visual styling, switch themes using the theme button.";

    QMessageBox::information(this, "Help", helpText);
}

