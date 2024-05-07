#include "chat_row_widget.h"

#include <QPainter>
#include <QPainterPath>

ChatRowWidget::ChatRowWidget(QWidget *parent)
	: QWidget{parent}
{
	initialize();
}

ChatRowWidget::ChatRowWidget(QWidget *parent, const QImage &avatar, const QString &displayName, const QString &post)
	: QWidget{parent}
{
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

void ChatRowWidget::initialize()
{
	setFixedHeight(80);
}

void ChatRowWidget::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);

	QColor bgColor = selected ? QColor(59, 104, 145) : QColor(36, 64, 89);

	QPen pen = selected ? QPen(QColor(255, 255, 255), 1) : Qt::NoPen;

	painter.setBrush(bgColor);
	painter.setPen(pen);
	painter.drawRoundedRect(rect(), 10, 10);

	//draw name
	QFont font("Segoe UI", 14, QFont::Normal);
	painter.setFont(font);
	painter.setPen(Qt::white);
	painter.drawText(rect().adjusted(80, 15, 0, 0), Qt::AlignLeft | Qt::AlignTop, displayName);

	//draw post
	QFont fontPost("Segoe UI", 12, QFont::Normal);
	painter.setFont(fontPost);
	painter.setPen(Qt::gray);
	painter.drawText(rect().adjusted(80, 35, 0, 0), Qt::AlignLeft | Qt::AlignTop, post);

	//draw round avatar
	QPixmap avatarPixmap = QPixmap::fromImage(avatar.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation));
	QPainterPath path;
	path.addRoundedRect(avatarPixmap.rect().adjusted(10, 10, 10, 10), 30, 30);
	painter.setClipPath(path);
	painter.drawPixmap(QRect(10, 10, 60, 60), avatarPixmap);
}

void ChatRowWidget::enterEvent(QEvent *)
{
	hovered = true;
}

void ChatRowWidget::leaveEvent(QEvent *)
{
	hovered = false;
}

void ChatRowWidget::mousePressEvent(QMouseEvent *)
{
	pressed = true;
}

void ChatRowWidget::mouseReleaseEvent(QMouseEvent *)
{
	pressed = false;
}
