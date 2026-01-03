#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "AppConstants.h"
#include "MenuToolBar.h"
#include "ThemeManager.h"

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

    void initMenuToolBar();

    // slotovi
    void onOpenGraphTriggered();
    void onCreateGraphTriggered();
    void onSaveGraphTriggered();
    void onSaveImageTriggered();
    void onChangeThemeTriggered();
};
#endif // MAINWINDOW_H
