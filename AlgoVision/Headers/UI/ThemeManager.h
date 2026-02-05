#ifndef THEME_H
#define THEME_H

#include <QFile>
#include <QSettings>
#include <QString>

class ThemeManager {
public:
    enum class Theme { LIGHT, DARK, PURPLE };

    explicit ThemeManager();

    Theme   currentTheme() const;
    QString styleSheet() const;

    void setTheme(Theme);

private:
    Theme     m_currentTheme {Theme::DARK};
    QSettings m_settings;

    void loadTheme();
    void saveTheme();
};

#endif // THEME_H
