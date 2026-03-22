#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), m_ui(new Ui::MainWindow), m_themeManager(new ThemeManager()),
      m_tabWidget(new QTabWidget(this)), m_serializer(std::make_unique<Serializer>()) {
    m_ui->setupUi(this);

    m_tabWidget->setTabsClosable(true);
    connect(m_tabWidget, &QTabWidget::tabCloseRequested, this, [this](int index) {
        QWidget* widget = m_tabWidget->widget(index);
        auto*    editor = qobject_cast<GraphEditor*>(widget);

        if(editor != nullptr) {
            TabInfo& tabInfo = m_tabs[editor];

            if(tabInfo.m_isModified) {
                QMessageBox::StandardButton reply = QMessageBox::question(
                    this, "Unsaved Changes",
                    QString("The graph in tab '%1' has unsaved changes. Do you want to save it?")
                        .arg(m_tabWidget->tabText(index)),
                    QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

                if(reply == QMessageBox::Yes) {
                    onSaveGraphTriggered();
                } else if(reply == QMessageBox::Cancel) {
                    return;
                }
            }

            m_tabWidget->removeTab(index);
            m_tabs.remove(editor);
            delete widget;
        }

        if(m_tabWidget->count() == 0) {
            showStartPage();
        }
    });

    m_ui->stackedWidget->addWidget(m_tabWidget);

    this->setStyleSheet(m_themeManager->styleSheet());

    showStartPage();

    this->setMinimumWidth(AppConstants::windowMinWidth);
    this->setMinimumHeight(AppConstants::windowMinHeight);
    this->resize(QSize(AppConstants::windowWidth, AppConstants::windowHeight));

    QWidget* startPage = m_ui->startPage;

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

void MainWindow::showStartPage() {
    m_ui->stackedWidget->setCurrentWidget(m_ui->startPage);
    this->setWindowTitle(QString::fromLatin1(AppConstants::startPageTitle));

    if(m_tabWidget != nullptr) {
        m_tabWidget->hide();
    }

    if(m_menuToolBar != nullptr) {
        m_menuToolBar->hide();
    }
}

void MainWindow::onOpenGraphTriggered() {
    QString filePath = QFileDialog::getOpenFileName(this, "open graph", "", "graph files (*.json)");

    if(filePath.isEmpty()) {
        return;
    }

    auto* loadThread = new LoadFileWorker(m_serializer.get(), filePath, this);

    connect(loadThread, &LoadFileWorker::loaded, this, &MainWindow::onGraphLoadedNewTab);

    connect(loadThread, &LoadFileWorker::failed, this, &MainWindow::onGraphLoadFailed);

    connect(loadThread, &QThread::finished, loadThread, &QObject::deleteLater);

    loadThread->start();

    initMenuToolBar();
}

void MainWindow::onCreateGraphTriggered() {
    QDialog dialog(this);
    dialog.setWindowTitle("Create Graph Options");
    dialog.setModal(true);
    dialog.setMinimumSize(AppConstants::createGraphDialogMinWidth,
                          AppConstants::createGraphDialogMinHeight);
    dialog.setSizeGripEnabled(true);
    dialog.setWindowFlags(dialog.windowFlags() | Qt::WindowMinMaxButtonsHint);

    QVBoxLayout* mainLayout = new QVBoxLayout(&dialog);

    QGroupBox*    typeGroup     = new QGroupBox("Graph Type", &dialog);
    QVBoxLayout*  typeLayout    = new QVBoxLayout(typeGroup);
    QRadioButton* directedBtn   = new QRadioButton("Directed", typeGroup);
    QRadioButton* undirectedBtn = new QRadioButton("Undirected", typeGroup);
    undirectedBtn->setChecked(true);
    typeLayout->addWidget(directedBtn);
    typeLayout->addWidget(undirectedBtn);
    mainLayout->addWidget(typeGroup);

    QGroupBox*    weightGroup   = new QGroupBox("Weight", &dialog);
    QVBoxLayout*  weightLayout  = new QVBoxLayout(weightGroup);
    QRadioButton* weightedBtn   = new QRadioButton("Weighted", weightGroup);
    QRadioButton* unweightedBtn = new QRadioButton("Unweighted", weightGroup);
    unweightedBtn->setChecked(true);
    weightLayout->addWidget(weightedBtn);
    weightLayout->addWidget(unweightedBtn);
    mainLayout->addWidget(weightGroup);

    QDialogButtonBox* buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
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
    } else {
        return;
    }

    auto controller = std::make_shared<GraphController>();
    controller->createGraph(directed, weighted);

    auto* editor = new GraphEditor(controller);
    controller->buildScene();
    editor->graphController()->scene()->applyTheme(m_themeManager->currentTheme());
    connectGraphModifiedSignal(editor);

    TabInfo info;
    info.m_editor          = editor;
    info.m_filePath        = "";
    info.m_isModified      = true;
    info.m_imagePath       = "";
    info.m_isImageModified = true;
    m_tabs.insert(editor, info);

    int index = m_tabWidget->addTab(editor, "untitled*");
    m_tabWidget->setCurrentIndex(index);

    m_tabWidget->setCurrentWidget(editor);

    if(!m_tabWidget->isVisible()) {
        m_tabWidget->show();
    }

    if(m_menuToolBar != nullptr) {
        m_menuToolBar->show();
    }

    m_ui->stackedWidget->setCurrentWidget(m_tabWidget);
    this->setWindowTitle(QString::fromLatin1(AppConstants::startPageTitle));

    initMenuToolBar();
}

void MainWindow::onSaveGraphTriggered() {
    auto* currentEditor = qobject_cast<GraphEditor*>(m_tabWidget->currentWidget());
    if(currentEditor == nullptr) {
        QMessageBox::warning(this, "error", "no active graph editor!");
        return;
    }

    int      index   = m_tabWidget->currentIndex();
    TabInfo& tabInfo = m_tabs[currentEditor];

    QString filePath = tabInfo.m_filePath;
    if(filePath.isEmpty()) {
        filePath = QFileDialog::getSaveFileName(this, "save graph", "", "graph files (*.json)");
        if(filePath.isEmpty()) {
            return;
        }
        if(!filePath.endsWith(".json")) {
            filePath += ".json";
        }
    }

    auto graph = currentEditor->graphController()->graph();
    if(graph == nullptr) {
        QMessageBox::warning(this, "error", "graph is not initialized!");
        return;
    }

    auto* saveThread = new SaveFileWorker(m_serializer.get(), graph.get(), filePath,
                                          graph->isWeighted(), graph->isDirected(), this);

    connect(saveThread, &SaveFileWorker::finished, this,
            [this, saveThread, &tabInfo, index, filePath]() {
                tabInfo.m_filePath   = filePath;
                tabInfo.m_isModified = false;
                m_tabWidget->setTabText(index, QFileInfo(filePath).fileName());
                m_tabWidget->setTabToolTip(index, filePath);
                QMessageBox::information(this, "saved", "graph saved to: " + filePath);
                saveThread->deleteLater();
            });

    connect(saveThread, &SaveFileWorker::failed, this, [this, saveThread](const QString& error) {
        QMessageBox::warning(this, "error", "could not save file: " + error);
        saveThread->deleteLater();
    });

    saveThread->start();
}

void MainWindow::onSaveImageTriggered() {
    auto* currentEditor = qobject_cast<GraphEditor*>(m_tabWidget->currentWidget());
    if(currentEditor == nullptr) {
        QMessageBox::warning(this, "error", "no active graph editor!");
        return;
    }

    int      index   = m_tabWidget->currentIndex();
    TabInfo& tabInfo = m_tabs[currentEditor];

    QString filePath = tabInfo.m_imagePath;
    if(filePath.isEmpty()) {
        filePath = QFileDialog::getSaveFileName(this, "save image", "", "image files (*.png)");
        if(filePath.isEmpty()) {
            return;
        }

        if(!filePath.endsWith(".png")) {
            filePath += ".png";
        }
    }

    QGraphicsView* view   = currentEditor->graphController()->scene()->views().first();
    QPixmap        pixmap = view->viewport()->grab();

    if(pixmap.save(filePath, "PNG")) {
        tabInfo.m_imagePath       = filePath;
        tabInfo.m_isImageModified = false;

        QString tabName = QFileInfo(filePath).fileName();

        if(tabInfo.m_isModified) {
            tabName += "*";
        }
        m_tabWidget->setTabText(index, tabName);
        m_tabWidget->setTabToolTip(index, tabInfo.m_imagePath.isEmpty() ? "unsaved image"
                                                                        : tabInfo.m_imagePath);

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

    for(auto it = m_tabs.begin(); it != m_tabs.end(); ++it) {
        auto* scene = it.value().m_editor->graphController()->scene();
        scene->applyTheme(m_themeManager->currentTheme());
    }
}

void MainWindow::onHelpTriggered() {
    QString helpText =
        "<h3>General info</h3>"

        "<p>"
        "This app is designed with the intention of being used as a learning tool and an "
        "interactive "
        "<b>visualizer</b> while studying graph algorithms."
        "</p>"

        "<ul>"

        "<li>"
        "Use buttons from the toolbar to change the theme colors of the app (purple, black, mint), "
        "to create a brand new graph, and even import one of yours as a <b>.json</b> file, "
        "to save your graph (as <b>.json</b>), or save an image of the scene with the colors of "
        "the current "
        "state of nodes and branches.<br>"
        "</li>"

        "<li>"
        "There are two tabs in the right menu — <b>Graph</b> and <b>Algorithm</b>."
        "<br><br>"
        "Use the <b>Graph tab</b> if you want to interact with the graph — enter add or delete "
        "mode "
        "(for nodes and branches), change the node size, "
        "or if you want to interact with the scene — undo, redo, zoom in/out, pan, or clear."
        "<br><br>"
        "Use the <b>Algorithm tab</b> if you want to select an algorithm, enter its attributes "
        "(start and/or end node), interact with the run algorithm buttons, use the "
        "<i>Graph type help</i>, or consult the color and results legend.<br>"
        "</li>"

        "<li>"
        "You have the scene on the left — that's where all the action is!<br>"
        "</li>"

        "</ul>"

        "<p>We hope this app helps &lt;3 </p>";

    QMessageBox::information(this, "Help", helpText);
}

void MainWindow::onGraphLoadedNewTab(const QVariant& data, bool weighted, bool directed,
                                     const QString& filePath) {
    auto controller = std::make_shared<GraphController>();
    controller->createGraph(directed, weighted);

    m_serializer->load(*controller->graph(), data);

    auto* editor = new GraphEditor(controller);
    controller->buildScene();
    editor->graphController()->scene()->applyTheme(m_themeManager->currentTheme());
    connectGraphModifiedSignal(editor);

    TabInfo info;
    info.m_editor          = editor;
    info.m_filePath        = filePath;
    info.m_isModified      = false;
    info.m_imagePath       = "";
    info.m_isImageModified = true;
    m_tabs.insert(editor, info);

    QString tabName = QFileInfo(filePath).fileName();
    int     index   = m_tabWidget->addTab(editor, tabName);
    m_tabWidget->setCurrentIndex(index);
    m_tabWidget->setTabToolTip(index, filePath.isEmpty() ? "unsaved file" : filePath);

    m_ui->stackedWidget->setCurrentWidget(m_tabWidget);
    this->setWindowTitle(QString::fromLatin1(AppConstants::startPageTitle));

    if(m_menuToolBar != nullptr) {
        m_menuToolBar->show();
    }
}

void MainWindow::onGraphLoadFailed(const QString& error) {
    QMessageBox::warning(this, "load error", error);
}

void MainWindow::connectGraphModifiedSignal(GraphEditor* editor) {
    connect(editor->graphController().get(), &GraphController::sceneModified, this,
            [this, editor]() {
                if(!m_tabs.contains(editor)) {
                    return;
                }

                TabInfo& tabInfo     = m_tabs[editor];
                tabInfo.m_isModified = true;

                QString tabName;
                if(tabInfo.m_filePath.isEmpty()) {
                    tabName = "untitled";
                } else {
                    tabName = QFileInfo(tabInfo.m_filePath).fileName();
                }

                if(!tabName.endsWith("*")) {
                    tabName += "*";
                }

                int index = m_tabWidget->indexOf(editor);
                if(index >= 0) {
                    m_tabWidget->setTabText(index, tabName);
                    m_tabWidget->setTabToolTip(
                        index, tabInfo.m_filePath.isEmpty() ? "unsaved file" : tabInfo.m_filePath);
                }
            });
}

void MainWindow::closeEvent(QCloseEvent* event) {
    QStringList unsavedTabs;

    // check all tabs
    for(auto it = m_tabs.begin(); it != m_tabs.end(); ++it) {
        if(it.value().m_isModified) {
            QString name;
            if(it.value().m_filePath.isEmpty()) {
                name = "untitled";
            } else {
                name = QFileInfo(it.value().m_filePath).fileName();
            }
            unsavedTabs.append(name);
        }
    }

    if(!unsavedTabs.isEmpty()) {
        QString                     tabList = unsavedTabs.join("\n");
        QMessageBox::StandardButton reply =
            QMessageBox::question(this, "Unsaved Changes",
                                  "The following tabs have unsaved changes:\n" + tabList +
                                      "\n\nDo you want to save them before exiting?",
                                  QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        if(reply == QMessageBox::Yes) {
            // save every unsaved tab
            for(auto it = m_tabs.begin(); it != m_tabs.end(); ++it) {
                if(it.value().m_isModified) {
                    m_tabWidget->setCurrentWidget(it.key());
                    onSaveGraphTriggered();
                }
            }
            event->accept();
        } else if(reply == QMessageBox::No) {
            event->accept();
        } else { // cancel
            event->ignore();
        }
    } else {
        event->accept();
    }
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
