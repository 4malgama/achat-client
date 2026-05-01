#ifndef THEME_DATA_H
#define THEME_DATA_H

#include <QString>
#include <QColor>
#include <QFont>

struct ThemeColors
{
	QColor windowBackground;
	QColor panelBackground;
	QColor cardBackground;

	QColor textPrimary;
	QColor textSecondary;
	QColor textMuted;

	QColor accent;
	QColor accentHover;
	QColor accentPressed;

	QColor border;
	QColor borderStrong;

	QColor inputBackground;
	QColor inputBorder;
	QColor inputFocusBorder;

	QColor avatarBorder;
	QColor avatarHoverOverlay;
};

struct ThemeMetrics
{
	int controlHeight = 36;
	int smallControlHeight = 28;
	int largeControlHeight = 44;

	int borderWidth = 1;

	int spacingXs = 4;
	int spacingSm = 8;
	int spacingMd = 12;
	int spacingLg = 16;
	int spacingXl = 24;

	int avatarSize = 200;
	int avatarBorderWidth = 2;
};

struct ThemeRadii
{
	int small = 6;
	int medium = 10;
	int large = 16;
	int round = 999;
};

struct ThemeFonts
{
	QFont base;
	QFont small;
	QFont button;
	QFont title;
};

struct ThemeData
{
	QString name;

	ThemeColors colors;
	ThemeMetrics metrics;
	ThemeRadii radii;
	ThemeFonts fonts;
};

#endif // THEME_DATA_H
