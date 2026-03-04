#include "chat_row_widget.h"

#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QLinearGradient>


ChatRowWidget::ChatRowWidget(QWidget *parent)
	: QWidget{parent}
{
	initialize();
}

ChatRowWidget::ChatRowWidget(QWidget *parent, quint64 chatId, const QImage &avatar, const QString &displayName, const QString &post)
	: QWidget{parent}
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

void ChatRowWidget::initialize()
{
	setFixedHeight(60);
	setCursor(Qt::PointingHandCursor);
}

void ChatRowWidget::paintEvent(QPaintEvent *)
{
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

	QLinearGradient penGradient(0, 0, width(), height());
	penGradient.setColorAt(0, QColor(249, 72, 134)); // #F94886
	penGradient.setColorAt(1, QColor(72, 198, 249)); // #48C6F9

	//QPen pen = selected ? QPen(penGradient, 2) : Qt::NoPen;
	QPen pen = Qt::NoPen;

	painter.setBrush(bgColor);
	painter.setPen(pen);
	painter.drawRect(rect());
	//painter.drawRoundedRect(rect(), 10, 10);

	//draw name
	QFont font("Segoe UI", 12, QFont::Normal);
	painter.setFont(font);
	painter.setPen(Qt::white);
	painter.drawText(rect().adjusted(height(), 10, 0, 0), Qt::AlignLeft | Qt::AlignTop, displayName);

	//draw post
	QFont fontPost("Segoe UI", 11, QFont::Normal);
	painter.setFont(fontPost);
	painter.setPen(Qt::gray);
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
