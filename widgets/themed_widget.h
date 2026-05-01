#ifndef THEMED_WIDGET_H
#define THEMED_WIDGET_H

#include <QWidget>

#include "../theme_manager/theme_manager.h"

class ThemedWidget : public QWidget
{
	Q_OBJECT
public:
	explicit ThemedWidget(QWidget *parent = nullptr);

protected:
	const ThemeData& theme() const;

	virtual void onThemeChanged(const ThemeData&);
};

#endif // THEMED_WIDGET_H
