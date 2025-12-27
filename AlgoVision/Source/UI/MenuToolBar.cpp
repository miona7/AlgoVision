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
}
