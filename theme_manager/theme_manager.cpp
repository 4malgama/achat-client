#include "theme_manager.h"
#include "theme_loader.h"

#include <QApplication>
#include <QFile>
#include <QDebug>


ThemeManager& ThemeManager::instance()
{
	static ThemeManager manager;
	return manager;
}

ThemeManager::ThemeManager(QObject* parent)
	: QObject(parent)
{
}

const ThemeData& ThemeManager::theme() const
{
	return m_theme;
}

bool ThemeManager::applyTheme(const QString& themeName)
{
	QString errorString;

	if (!ThemeLoader::loadTheme(themeName, m_theme, &errorString))
	{
		qWarning() << errorString;

		qApp->setStyleSheet("");
		emit themeChanged(m_theme);

		return false;
	}

	const QString qssPath = ":/r/themes/" + themeName + "/" + themeName + ".qss";

	QFile qssFile(qssPath);
	if (!qssFile.open(QFile::ReadOnly | QFile::Text))
	{
		qWarning() << "Cannot open theme qss:" << qssPath;

		qApp->setFont(m_theme.fonts.base);
		qApp->setStyleSheet("");

		emit themeChanged(m_theme);
		return false;
	}

	const QString qss = QString::fromUtf8(qssFile.readAll());

	qApp->setFont(m_theme.fonts.base);
	qApp->setStyleSheet(qss);

	emit themeChanged(m_theme);
	return true;
}
