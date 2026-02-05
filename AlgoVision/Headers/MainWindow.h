#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHash>
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
#include <QTabWidget>
#include <QCloseEvent>

#include <memory>
#include <iostream>

#include "AppConstants.h"
#include "MenuToolBar.h"
#include "ThemeManager.h"
#include "GraphController.h"
#include "Graph.h"
#include "GraphEditor.h"
#include "Serializer.h"
#include "UnweightedDirectedGraph.h"
#include "UnweightedUndirectedGraph.h"
#include "WeightedDirectedGraph.h"
#include "WeightedUndirectedGraph.h"
#include "LoadFileWorker.h"
#include "SaveFileWorker.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    struct TabInfo {
        GraphEditor* m_editor;
        QString m_filePath;
        bool m_isModified;
        QString m_imagePath;
        bool m_isImageModified;
    };

    explicit MainWindow(QWidget* = nullptr);
    ~MainWindow() override;

    void showStartPage();

private slots:
    void onOpenGraphTriggered();
    void onCreateGraphTriggered();
    void onSaveGraphTriggered();
    void onSaveImageTriggered();
    void onChangeThemeTriggered();
    void onHelpTriggered();

    void onGraphLoadedNewTab(const QVariant&, bool, bool, const QString&);
    void onGraphLoadFailed(const QString&);
    void connectGraphModifiedSignal(GraphEditor*);

protected:
    void closeEvent(QCloseEvent*) override;

private:
    Ui::MainWindow* m_ui;
    MenuToolBar*    m_menuToolBar = nullptr;
    ThemeManager*   m_themeManager;
    QTabWidget* m_tabWidget = nullptr;
    QHash<GraphEditor*, TabInfo> m_tabs;

    std::unique_ptr<Serializer> m_serializer;

    void initMenuToolBar();
};

#endif // MAINWINDOW_H
