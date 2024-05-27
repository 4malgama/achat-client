#include "dropdown_menu.h"
#include <QPropertyAnimation>
#include <QShowEvent>
#include <QPainter>


DropdownMenu::DropdownMenu(QWidget *parent)
	: QMenu{ parent }
{
	toRight = true;
	animation = new QPropertyAnimation(this, "geometry");
	animation->setDuration(400);
	animation->setEasingCurve(QEasingCurve::OutCubic);
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

void DropdownMenu::paintEvent(QPaintEvent *)
{
	QPainter painter(this);

	//background
	painter.setBrush(QColor(65, 110, 150));
	painter.setPen(Qt::NoPen);
	painter.drawRect(rect());

	//selected item
	QAction* selectedAction = activeAction();
	if (selectedAction != nullptr)
	{
		QRect actionRect = actionGeometry(selectedAction);
		painter.setBrush(QColor(85, 130, 170));
		painter.setPen(Qt::NoPen);
		painter.drawRect(actionRect);
	}

	//texts
	for (QAction* action : actions())
	{
		QRect actionRect = actionGeometry(action);
		painter.setPen(QPen(Qt::white));
		painter.setFont(QFont("Segoe UI", 10));
		painter.drawText(actionRect, Qt::AlignVCenter | Qt::AlignLeft, action->text());
	}
}
