#ifndef THEME_BINDING_H
#define THEME_BINDING_H

#include "theme_manager.h"

#include <QWidget>
#include <functional>


namespace theme
{
	inline const ThemeData& current()
	{
		return ThemeManager::instance().theme();
	}

	inline void bind(QWidget* widget, std::function<void(const ThemeData&)> callback)
	{
		QObject::connect(&ThemeManager::instance(), &ThemeManager::themeChanged, widget, [widget, callback] (const ThemeData& theme) mutable
		{
			callback(theme);
			widget->updateGeometry();
			widget->update();
		});

		callback(ThemeManager::instance().theme());
	}
}

#endif // THEME_BINDING_H
