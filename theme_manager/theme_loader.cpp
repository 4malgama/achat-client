#include "theme_loader.h"

#include <QtGlobal>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>

static QFont::Weight cssWeightToQtWeight(int cssWeight)
{
	if (cssWeight <= 100)
		return QFont::Thin;
	if (cssWeight <= 200)
		return QFont::ExtraLight;
	if (cssWeight <= 300)
		return QFont::Light;
	if (cssWeight <= 400)
		return QFont::Normal;
	if (cssWeight <= 500)
		return QFont::Medium;
	if (cssWeight <= 600)
		return QFont::DemiBold;
	if (cssWeight <= 700)
		return QFont::Bold;
	if (cssWeight <= 800)
		return QFont::ExtraBold;

	return QFont::Black;
}

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
	theme.colors.surfaceBackground = readColor(colors, "surfaceBackground", theme.colors.panelBackground);
	theme.colors.surfaceHover = readColor(colors, "surfaceHover", theme.colors.surfaceBackground.lighter(115));
	theme.colors.surfacePressed = readColor(colors, "surfacePressed", theme.colors.surfaceBackground.lighter(125));
	theme.colors.cardBackground = readColor(colors, "cardBackground");

	theme.colors.textPrimary = readColor(colors, "textPrimary");
	theme.colors.textSecondary = readColor(colors, "textSecondary");
	theme.colors.textMuted = readColor(colors, "textMuted");
	theme.colors.textOnAccent = readColor(colors, "textOnAccent", theme.colors.textPrimary);

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

	theme.colors.success = readColor(colors, "success", QColor("#22C55E"));
	theme.colors.warning = readColor(colors, "warning", QColor("#F59E0B"));
	theme.colors.danger = readColor(colors, "danger", QColor("#EF4444"));
	theme.colors.consoleText = readColor(colors, "consoleText", theme.colors.success);

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

	theme.metrics.chatBubbleMaxWidthPercent = readInt(metrics, "chatBubbleMaxWidthPercent", 40);
	theme.metrics.chatBubblePaddingX = readInt(metrics, "chatBubblePaddingX", 12);
	theme.metrics.chatBubblePaddingY = readInt(metrics, "chatBubblePaddingY", 8);
	theme.metrics.chatBubbleSpacing = readInt(metrics, "chatBubbleSpacing", 8);
	theme.metrics.chatBubbleDateHeight = readInt(metrics, "chatBubbleDateHeight", 18);
	theme.metrics.chatBubbleMinWidth = readInt(metrics, "chatBubbleMinWidth", 120);
	theme.metrics.attachmentButtonHeight = readInt(metrics, "attachmentButtonHeight", 34);

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
	theme.fonts.monospace = readFont(fonts, "monospace", QFont("Cascadia Mono", 10));
	theme.fonts.message = readFont(fonts, "message", theme.fonts.base);
	theme.fonts.messageDate = readFont(fonts, "messageDate", theme.fonts.small);

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
	const int cssWeight = fontObject.value("weight").toInt(400);

	QFont font(family, pointSize);
	font.setWeight(cssWeightToQtWeight(cssWeight));

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
