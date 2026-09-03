#ifndef THEME_LOADER_H
#define THEME_LOADER_H

#include "theme_data.h"

#include <QString>
#include <QJsonObject>
#include <QJsonValue>

class ThemeLoader
{
public:
	static bool loadTheme(const QString& themeName, ThemeData& outTheme, QString* errorString = nullptr);

private:
	static QColor readColor(const QJsonObject& object, const QString& key, const QColor& fallback = QColor());
	static int readInt(const QJsonObject& object, const QString& key, int fallback);
	static QFont readFont(const QJsonObject& object, const QString& key, const QFont& fallback = QFont());

	static QColor colorFromJsonValue(const QJsonValue& value, const QColor& fallback);
};

#endif // THEME_LOADER_H
