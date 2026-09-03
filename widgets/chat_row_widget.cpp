#include "chat_row_widget.h"

#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QLinearGradient>


ChatRowWidget::ChatRowWidget(QWidget *parent)
	: ThemedWidget{parent}
{
	initialize();
}

ChatRowWidget::ChatRowWidget(QWidget *parent, quint64 chatId, const QImage &avatar, const QString &displayName, const QString &post)
	: ThemedWidget{parent}
{
	this->chatId = chatId;
	this->avatar = avatar;
	this->displayName = displayName;
	this->post = post;
	initialize();
}

void ChatRowWidget::setAvatar(const QImage &avatar)
{
	this->avatar = avatar;
}

void ChatRowWidget::setDisplayName(const QString &displayName)
{
	this->displayName = displayName;
}

void ChatRowWidget::setPost(const QString &post)
{
	this->post = post;
}

QImage ChatRowWidget::getAvatar()
{
	return avatar;
}

QString ChatRowWidget::getDisplayName()
{
	return displayName;
}

QString ChatRowWidget::getPost()
{
	return post;
}

void ChatRowWidget::setSelected(bool state)
{
	selected = state;
	update();
}

bool ChatRowWidget::getSelected()
{
	return selected;
}

void ChatRowWidget::click()
{
	emit clicked();
}

void ChatRowWidget::onThemeChanged(const ThemeData &theme)
{
	setFont(theme.fonts.base);

	updateGeometry();
	update();
}

void ChatRowWidget::initialize()
{
	setFixedHeight(60);
	setCursor(Qt::PointingHandCursor);
}

void ChatRowWidget::paintEvent(QPaintEvent *)
{
	const ThemeData& t = theme();

	QPainter painter(this);
	painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);

	QColor bgColor = selected ? colors.bg.selected : colors.bg.common;

	if (pressed)
	{
		bgColor = colors.bg.pressed;
	}
	else if (hovered)
	{
		bgColor = colors.bg.hovered;
	}

	QPen pen = Qt::NoPen;

	painter.setBrush(bgColor);
	painter.setPen(pen);
	painter.drawRect(rect());

	//draw name
	painter.setFont(t.fonts.base);
	painter.setPen(t.colors.textPrimary);
	painter.drawText(rect().adjusted(height(), 10, 0, 0), Qt::AlignLeft | Qt::AlignTop, displayName);

	//draw post
	painter.setFont(t.fonts.base);
	painter.setPen(t.colors.textSecondary);
	painter.drawText(rect().adjusted(height(), 30, 0, 0), Qt::AlignLeft | Qt::AlignTop, post);

	//draw round avatar
	QPixmap avatarPixmap = QPixmap::fromImage(avatar.scaled(height() * 0.75f, height() * 0.75f, Qt::KeepAspectRatio, Qt::SmoothTransformation));
	QPainterPath path;
	path.addRoundedRect(avatarPixmap.rect().adjusted(8, 8, 8, 8), 22, 22);
	painter.setClipPath(path);
	painter.drawPixmap(QRect(8, 8, height() * 0.75f, height() * 0.75f), avatarPixmap);
}

void ChatRowWidget::enterEvent(QEvent *)
{
	hovered = true;
	update();
}

void ChatRowWidget::leaveEvent(QEvent *)
{
	hovered = false;
	update();
}

void ChatRowWidget::mousePressEvent(QMouseEvent *e)
{
	pressed = e->buttons() & Qt::LeftButton;
	if (pressed) emit clicked();
	update();
}

void ChatRowWidget::mouseReleaseEvent(QMouseEvent *e)
{
	pressed = e->buttons() &~ Qt::LeftButton;
	update();
}
