#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "MenuToolBar.h"
#include "AppConstants.h"

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

private slots:
    void onOpenGraphTriggered();
    void onCreateGraphTriggered();

private:
    Ui::MainWindow* m_ui;
    MenuToolBar* m_menuToolBar = nullptr;

    void initMenuToolBar();
};
#endif // MAINWINDOW_H
