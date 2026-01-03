#include "ThemeManager.h"

ThemeManager::ThemeManager() : m_currentTheme(Theme::DARK), m_settings("MyCompany", "MyApp") {
    loadTheme();
}

ThemeManager::Theme ThemeManager::currentTheme() const {
    return m_currentTheme;
}

QString ThemeManager::styleSheet() const {
    QString path;
    switch(m_currentTheme) {
        case Theme::DARK:
            path = ":themes/dark.qss";
            break;
        case Theme::LIGHT:
            path = ":themes/light.qss";
            break;
        case Theme::PURPLE:
            path = ":themes/purple.qss";
            break;
    }

    QFile file(path);
    if(!file.open(QIODevice::ReadOnly)) {
        qWarning() << "could not open QSS file:" << path;
        return "";
    }

    QString style = file.readAll();
    file.close();
    return style;
}

void ThemeManager::setTheme(Theme theme) {
    m_currentTheme = theme;
    saveTheme();
}

void ThemeManager::loadTheme() {
    QString t = m_settings.value("theme", "dark").toString();
    if(t == "dark") {
        m_currentTheme = Theme::DARK;
    } else if(t == "light") {
        m_currentTheme = Theme::LIGHT;
    } else if(t == "purple") {
        m_currentTheme = Theme::PURPLE;
    }
}

void ThemeManager::saveTheme() {
    QString t;
    switch(m_currentTheme) {
        case Theme::DARK:
            t = "dark";
            break;
        case Theme::LIGHT:
            t = "light";
            break;
        case Theme::PURPLE:
            t = "purple";
            break;
    }

    m_settings.setValue("theme", t);
}
