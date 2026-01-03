#ifndef THEME_H
#define THEME_H

#include <QString>
#include <QSettings>
#include <QFile>

class ThemeManager {
public:
    enum class Theme {
        LIGHT,
        DARK,
        PURPLE
    };

    explicit ThemeManager();

    Theme currentTheme() const;
    QString styleSheet() const; // vraca QString sa CSS-om za trenutnu temu

    void setTheme(Theme);

private:
    Theme m_currentTheme;
    QSettings m_settings; // naredno pokretanje -> sacuvana tema

    void loadTheme();
    void saveTheme();
};

#endif // THEME_H
