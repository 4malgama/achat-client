#include "dropdown_menu.h"
#include "../theme_manager/theme_binding.h"
#include <QPropertyAnimation>
#include <QShowEvent>


DropdownMenu::DropdownMenu(QWidget *parent)
	: QMenu{ parent }
{
	toRight = true;
	animation = new QPropertyAnimation(this, "geometry");
	animation->setDuration(400);
	animation->setEasingCurve(QEasingCurve::OutCubic);

	theme::bind(this, [this] (const ThemeData& theme) {
		setFont(theme.fonts.base);
		setContentsMargins(
			theme.metrics.spacingXs,
			theme.metrics.spacingXs,
			theme.metrics.spacingXs,
			theme.metrics.spacingXs
		);
	});
}

QAction *DropdownMenu::makeAction(const QString &text, QMenu *parentMenu, void (*slot)())
{
	QAction* newAction = new QAction(text, parentMenu);
	QObject::connect(newAction, &QAction::triggered, slot);
	return newAction;
}

void DropdownMenu::showEvent(QShowEvent *e)
{
	QRect startGeometry;
	QRect endGeometry;
	if (toRight)
	{
		startGeometry = QRect(pos().x(), pos().y(), 0, 0);
		endGeometry = QRect(pos().x(), pos().y(), width(), height());
	}
	else
	{
		startGeometry = QRect(pos().x() + width(), pos().y(), 0, 0);
		endGeometry = QRect(pos().x(), pos().y(), width(), height());
	}

	animation->setStartValue(startGeometry);
	animation->setEndValue(endGeometry);

	animation->start();
	QMenu::showEvent(e);
}
