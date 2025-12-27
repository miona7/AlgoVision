#include "../Headers/UI/MenuToolBar.h"


MenuToolBar::MenuToolBar(QWidget* parent)
    : QToolBar{parent},
      m_createGraphAction(new QAction("create graph", this)),
      m_openGraphAction(new QAction("open graph", this)),
      m_saveGraphAction(new QAction("save graph", this)),
      m_saveImageAction(new QAction("save image", this)),
      m_changeThemeAction(new QAction("change theme", this)),
      m_helpAction(new QAction("help", this)) {
    addAction(m_createGraphAction);
    addAction(m_openGraphAction);
    addAction(m_saveGraphAction);
    addAction(m_saveImageAction);
    addAction(m_changeThemeAction);
    addAction(m_helpAction);

    setStyle();
    setOptionShortcuts();
}

void MenuToolBar::setStyle() {
    setIconSize(QSize(64, 64));
    QString style = QString("QToolButton {width: %1px; height: %2px;}").arg(90).arg(90);
    setStyleSheet(style);
}

void MenuToolBar::setOptionShortcuts() {
    m_createGraphAction->setShortcut(QKeySequence::New);
    m_createGraphAction->setToolTip("create graph (Ctrl + N)");

    m_openGraphAction->setShortcut(QKeySequence::Open);
    m_openGraphAction->setToolTip("open graph (Ctrl + O)");

    m_saveGraphAction->setShortcut(QKeySequence::Save);
    m_saveGraphAction->setToolTip("save graph (Ctrl + S)");

    m_saveImageAction->setShortcut(Qt::CTRL | Qt::Key_I);
    m_saveImageAction->setToolTip("save image (Ctrl + I)");

    m_helpAction->setShortcut(Qt::CTRL | Qt::Key_H);
    m_helpAction->setToolTip("help (Ctrl + H)");

    m_changeThemeAction->setShortcut(Qt::CTRL | Qt::Key_T);
    m_changeThemeAction->setToolTip("change theme (Ctrl + T)");
}
