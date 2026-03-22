#ifndef MENUTOOLBAR_H
#define MENUTOOLBAR_H

#include <QToolBar>

#include "AppConstants.h"

class QAction;

class MenuToolBar : public QToolBar {
    Q_OBJECT
public:
    explicit MenuToolBar(QWidget* = nullptr);
    ~MenuToolBar() override = default;

    QAction* createGraphAction() const;
    QAction* openGraphAction() const;
    QAction* saveGraphAction() const;
    QAction* saveImageAction() const;
    QAction* changeThemeAction() const;
    QAction* helpAction() const;

private:
    QAction* m_createGraphAction{nullptr};
    QAction* m_openGraphAction{nullptr};
    QAction* m_saveGraphAction{nullptr};
    QAction* m_saveImageAction{nullptr};
    QAction* m_changeThemeAction{nullptr};
    QAction* m_helpAction{nullptr};

    void setStyle();
    void setOptionShortcuts();
};

#endif // MENUTOOLBAR_H
