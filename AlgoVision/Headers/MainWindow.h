#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>

#include "AppConstants.h"
#include "MenuToolBar.h"
#include "ThemeManager.h"

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
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow* m_ui;
    MenuToolBar*    m_menuToolBar = nullptr;
    ThemeManager*   m_themeManager;

    std::unique_ptr<Serializer> m_serializer;
    std::shared_ptr<Graph>      m_graph;
    GraphEditor*                m_graphEditor = nullptr;

    void                   initMenuToolBar();
    std::shared_ptr<Graph> createGraph(bool, bool);

    // slotovi
    void onOpenGraphTriggered();
    void onCreateGraphTriggered();
    void onSaveGraphTriggered();
    void onSaveImageTriggered();
    void onChangeThemeTriggered();
};
#endif // MAINWINDOW_H
