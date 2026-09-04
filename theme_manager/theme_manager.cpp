#include "theme_manager.h"
#include "theme_loader.h"

#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QHash>
#include <QPalette>
#include <QRegularExpression>


namespace
{
	QString colorToQss(const QColor& color)
	{
		if (!color.isValid())
			return QStringLiteral("transparent");

		return QStringLiteral("rgba(%1, %2, %3, %4)")
			.arg(color.red())
			.arg(color.green())
			.arg(color.blue())
			.arg(color.alpha());
	}

	QString expandStyleSheet(QString qss, const ThemeData& theme)
	{
		const ThemeColors& c = theme.colors;
		const ThemeMetrics& m = theme.metrics;
		const ThemeRadii& r = theme.radii;

		const QHash<QString, QString> values = {
			{ QStringLiteral("windowBackground"), colorToQss(c.windowBackground) },
			{ QStringLiteral("panelBackground"), colorToQss(c.panelBackground) },
			{ QStringLiteral("surfaceBackground"), colorToQss(c.surfaceBackground) },
			{ QStringLiteral("surfaceHover"), colorToQss(c.surfaceHover) },
			{ QStringLiteral("surfacePressed"), colorToQss(c.surfacePressed) },
			{ QStringLiteral("cardBackground"), colorToQss(c.cardBackground) },
			{ QStringLiteral("textPrimary"), colorToQss(c.textPrimary) },
			{ QStringLiteral("textSecondary"), colorToQss(c.textSecondary) },
			{ QStringLiteral("textMuted"), colorToQss(c.textMuted) },
			{ QStringLiteral("textOnAccent"), colorToQss(c.textOnAccent) },
			{ QStringLiteral("accent"), colorToQss(c.accent) },
			{ QStringLiteral("accentHover"), colorToQss(c.accentHover) },
			{ QStringLiteral("accentPressed"), colorToQss(c.accentPressed) },
			{ QStringLiteral("border"), colorToQss(c.border) },
			{ QStringLiteral("borderStrong"), colorToQss(c.borderStrong) },
			{ QStringLiteral("inputBackground"), colorToQss(c.inputBackground) },
			{ QStringLiteral("inputBorder"), colorToQss(c.inputBorder) },
			{ QStringLiteral("inputFocusBorder"), colorToQss(c.inputFocusBorder) },
			{ QStringLiteral("success"), colorToQss(c.success) },
			{ QStringLiteral("warning"), colorToQss(c.warning) },
			{ QStringLiteral("danger"), colorToQss(c.danger) },
			{ QStringLiteral("consoleText"), colorToQss(c.consoleText) },
			{ QStringLiteral("borderWidth"), QString::number(m.borderWidth) },
			{ QStringLiteral("smallControlContentHeight"), QString::number(qMax(0, m.smallControlHeight - 2 * m.borderWidth)) },
			{ QStringLiteral("spacingXs"), QString::number(m.spacingXs) },
			{ QStringLiteral("spacingSm"), QString::number(m.spacingSm) },
			{ QStringLiteral("spacingMd"), QString::number(m.spacingMd) },
			{ QStringLiteral("radiusSmall"), QString::number(r.small) },
			{ QStringLiteral("radiusMedium"), QString::number(r.medium) },
			{ QStringLiteral("radiusLarge"), QString::number(r.large) }
		};

		for (auto it = values.cbegin(); it != values.cend(); ++it)
			qss.replace(QStringLiteral("{{") + it.key() + QStringLiteral("}}"), it.value());

		return qss;
	}

	QPalette buildPalette(const ThemeData& theme)
	{
		QPalette palette;
		palette.setColor(QPalette::Window, theme.colors.windowBackground);
		palette.setColor(QPalette::WindowText, theme.colors.textPrimary);
		palette.setColor(QPalette::Base, theme.colors.inputBackground);
		palette.setColor(QPalette::AlternateBase, theme.colors.surfaceBackground);
		palette.setColor(QPalette::Text, theme.colors.textPrimary);
		palette.setColor(QPalette::Button, theme.colors.surfaceBackground);
		palette.setColor(QPalette::ButtonText, theme.colors.textPrimary);
		palette.setColor(QPalette::ToolTipBase, theme.colors.surfaceBackground);
		palette.setColor(QPalette::ToolTipText, theme.colors.textPrimary);
		palette.setColor(QPalette::Highlight, theme.colors.accent);
		palette.setColor(QPalette::HighlightedText, theme.colors.textOnAccent);
		palette.setColor(QPalette::Disabled, QPalette::WindowText, theme.colors.textMuted);
		palette.setColor(QPalette::Disabled, QPalette::Text, theme.colors.textMuted);
		palette.setColor(QPalette::Disabled, QPalette::ButtonText, theme.colors.textMuted);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
		palette.setColor(QPalette::PlaceholderText, theme.colors.textMuted);
#endif
		return palette;
	}
}


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

	const QString qss = expandStyleSheet(QString::fromUtf8(qssFile.readAll()), m_theme);
	const QRegularExpression unresolvedToken(QStringLiteral("\\{\\{[^}]+\\}\\}"));
	if (qss.contains(unresolvedToken))
	{
		qWarning() << "Theme qss contains an unknown token:" << unresolvedToken.match(qss).captured();
		qApp->setStyleSheet("");
		emit themeChanged(m_theme);
		return false;
	}

	qApp->setFont(m_theme.fonts.base);
	qApp->setPalette(buildPalette(m_theme));
	qApp->setStyleSheet(qss);

	emit themeChanged(m_theme);
	return true;
}
