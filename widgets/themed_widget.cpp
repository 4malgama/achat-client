#include "themed_widget.h"


ThemedWidget::ThemedWidget(QWidget *parent)
	: QWidget{parent}
{
	connect(
		&ThemeManager::instance(),
		&ThemeManager::themeChanged,
		this,
		[this](const ThemeData& theme) {
			onThemeChanged(theme);
		}
	);
}

const ThemeData &ThemedWidget::theme() const
{
	return ThemeManager::instance().theme();
}

void ThemedWidget::onThemeChanged(const ThemeData &)
{
	updateGeometry();
	update();
}
