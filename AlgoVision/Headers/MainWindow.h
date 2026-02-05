#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHash>
#include <memory>

#include "AppConstants.h"
#include "MenuToolBar.h"
#include "ThemeManager.h"
#include "GraphController.h"

class Graph;
class Serializer;
class GraphEditor;

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
        QString m_filePath;   // prazan ako je novi / untitled
        bool m_isModified;
        QString m_imagePath;  // fajl slike (ako se eksportuje)
        bool m_isImageModified; // za sliku (zvezdica)
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
