#include "theme_loader.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>


bool ThemeLoader::loadTheme(const QString& themeName, ThemeData& outTheme, QString* errorString)
{
	const QString jsonPath = ":/r/themes/" + themeName + "/" + themeName + ".json";

	QFile file(jsonPath);
	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		if (errorString)
			*errorString = "Cannot open theme json: " + jsonPath;

		return false;
	}

	QJsonParseError parseError;
	const QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &parseError);

	if (parseError.error != QJsonParseError::NoError)
	{
		if (errorString)
			*errorString = "Theme json parse error: " + parseError.errorString();

		return false;
	}

	if (!doc.isObject())
	{
		if (errorString)
			*errorString = "Theme json root must be object.";

		return false;
	}

	const QJsonObject root = doc.object();

	ThemeData theme;
	theme.name = root.value("name").toString(themeName);

	const QJsonObject colors = root.value("colors").toObject();

	theme.colors.windowBackground = readColor(colors, "windowBackground");
	theme.colors.panelBackground = readColor(colors, "panelBackground");
	theme.colors.cardBackground = readColor(colors, "cardBackground");

	theme.colors.textPrimary = readColor(colors, "textPrimary");
	theme.colors.textSecondary = readColor(colors, "textSecondary");
	theme.colors.textMuted = readColor(colors, "textMuted");

	theme.colors.accent = readColor(colors, "accent");
	theme.colors.accentHover = readColor(colors, "accentHover");
	theme.colors.accentPressed = readColor(colors, "accentPressed");

	theme.colors.border = readColor(colors, "border");
	theme.colors.borderStrong = readColor(colors, "borderStrong");

	theme.colors.inputBackground = readColor(colors, "inputBackground");
	theme.colors.inputBorder = readColor(colors, "inputBorder");
	theme.colors.inputFocusBorder = readColor(colors, "inputFocusBorder");

	theme.colors.avatarBorder = readColor(colors, "avatarBorder");
	theme.colors.avatarHoverOverlay = readColor(colors, "avatarHoverOverlay");

	const QJsonObject metrics = root.value("metrics").toObject();

	theme.metrics.controlHeight = readInt(metrics, "controlHeight", 36);
	theme.metrics.smallControlHeight = readInt(metrics, "smallControlHeight", 28);
	theme.metrics.largeControlHeight = readInt(metrics, "largeControlHeight", 44);

	theme.metrics.borderWidth = readInt(metrics, "borderWidth", 1);

	theme.metrics.spacingXs = readInt(metrics, "spacingXs", 4);
	theme.metrics.spacingSm = readInt(metrics, "spacingSm", 8);
	theme.metrics.spacingMd = readInt(metrics, "spacingMd", 12);
	theme.metrics.spacingLg = readInt(metrics, "spacingLg", 16);
	theme.metrics.spacingXl = readInt(metrics, "spacingXl", 24);

	theme.metrics.avatarSize = readInt(metrics, "avatarSize", 200);
	theme.metrics.avatarBorderWidth = readInt(metrics, "avatarBorderWidth", 2);

	const QJsonObject radii = root.value("radii").toObject();

	theme.radii.small = readInt(radii, "small", 6);
	theme.radii.medium = readInt(radii, "medium", 10);
	theme.radii.large = readInt(radii, "large", 16);
	theme.radii.round = readInt(radii, "round", 999);

	const QJsonObject fonts = root.value("fonts").toObject();

	theme.fonts.base = readFont(fonts, "base", QFont("Segoe UI", 10));
	theme.fonts.small = readFont(fonts, "small", QFont("Segoe UI", 9));
	theme.fonts.button = readFont(fonts, "button", QFont("Segoe UI", 10, QFont::Medium));
	theme.fonts.title = readFont(fonts, "title", QFont("Segoe UI", 16, QFont::DemiBold));

	outTheme = theme;
	return true;
}

QColor ThemeLoader::readColor(const QJsonObject& object, const QString& key, const QColor& fallback)
{
	return colorFromJsonValue(object.value(key), fallback);
}

int ThemeLoader::readInt(const QJsonObject& object, const QString& key, int fallback)
{
	const QJsonValue value = object.value(key);

	if (!value.isDouble())
		return fallback;

	return value.toInt(fallback);
}

QFont ThemeLoader::readFont(const QJsonObject& object, const QString& key, const QFont& fallback)
{
	const QJsonObject fontObject = object.value(key).toObject();

	if (fontObject.isEmpty())
		return fallback;

	const QString family = fontObject.value("family").toString(fallback.family());
	const int pointSize = fontObject.value("pointSize").toInt(fallback.pointSize());
	const int weight = fontObject.value("weight").toInt(fallback.weight());

	QFont font(family, pointSize);
	font.setWeight(weight);

	return font;
}

QColor ThemeLoader::colorFromJsonValue(const QJsonValue& value, const QColor& fallback)
{
	if (value.isString())
	{
		const QColor color(value.toString());

		if (color.isValid())
			return color;

		return fallback;
	}

	if (value.isArray())
	{
		const QJsonArray arr = value.toArray();

		if (arr.size() == 3)
		{
			return QColor(
				arr.at(0).toInt(),
				arr.at(1).toInt(),
				arr.at(2).toInt()
			);
		}

		if (arr.size() == 4)
		{
			return QColor(
				arr.at(0).toInt(),
				arr.at(1).toInt(),
				arr.at(2).toInt(),
				arr.at(3).toInt()
			);
		}
	}

	return fallback;
}
