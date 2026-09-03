#ifndef THEME_MANAGER_H
#define THEME_MANAGER_H

#include "theme_data.h"

#include <QObject>
#include <QString>

class ThemeManager : public QObject
{
	Q_OBJECT

public:
	static ThemeManager& instance();

	const ThemeData& theme() const;

	bool applyTheme(const QString& themeName);

signals:
	void themeChanged(const ThemeData& theme);

private:
	explicit ThemeManager(QObject* parent = nullptr);

private:
	ThemeData m_theme;
};

#endif // THEME_MANAGER_H
