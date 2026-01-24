#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
    explicit MainWindow(QWidget* = nullptr);
    ~MainWindow() override;

private slots:
    void onOpenGraphTriggered();
    void onCreateGraphTriggered();
    void onSaveGraphTriggered();
    void onSaveImageTriggered();
    void onChangeThemeTriggered();
    void onHelpTriggered(); // <- slot za Help dugme

private:
    Ui::MainWindow* m_ui;
    MenuToolBar*    m_menuToolBar = nullptr;
    ThemeManager*   m_themeManager;

    std::unique_ptr<Serializer> m_serializer;
    std::shared_ptr<Graph>      m_graph;
    GraphEditor*                m_graphEditor = nullptr;

    void                   initMenuToolBar();
    void createGraphEditor(bool, bool);

    // ukloni ovaj metod, treba i vec postoji u okviru kontolera
    std::shared_ptr<Graph> createGraph(bool, bool);
};

#endif // MAINWINDOW_H

