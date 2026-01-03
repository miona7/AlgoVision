#ifndef MENUTOOLBAR_H
#define MENUTOOLBAR_H

#include <QToolBar>

#include "AppConstants.h"

class QAction;

class MenuToolBar : public QToolBar {
    Q_OBJECT

public:
    explicit MenuToolBar(QWidget* parent = nullptr);
    ~MenuToolBar() override = default;

private:
    QAction* m_createGraphAction;
    QAction* m_openGraphAction;
    QAction* m_saveGraphAction;
    QAction* m_saveImageAction;
    QAction* m_changeThemeAction;
    QAction* m_helpAction;

    void setStyle();
    void setOptionShortcuts();
};

#endif // MENUTOOLBAR_H
