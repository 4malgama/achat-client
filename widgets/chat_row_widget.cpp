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
	update();
}

void ChatRowWidget::setDisplayName(const QString &displayName)
{
	this->displayName = displayName;
	update();
}

void ChatRowWidget::setPost(const QString &post)
{
	this->post = post;
	update();
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
	ThemedWidget::onThemeChanged(theme);
}

void ChatRowWidget::initialize()
{
	setFixedHeight(60);
	setCursor(Qt::PointingHandCursor);
	onThemeChanged(theme());
}

void ChatRowWidget::paintEvent(QPaintEvent *)
{
	const ThemeData& t = theme();

	QPainter painter(this);
	painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);

	QColor bgColor = selected ? t.colors.accentPressed : t.colors.surfaceBackground;

	if (pressed)
	{
		bgColor = t.colors.surfacePressed;
	}
	else if (hovered)
	{
		bgColor = t.colors.surfaceHover;
	}

	painter.setBrush(bgColor);
	painter.setPen(QPen(selected ? t.colors.accentHover : t.colors.border, t.metrics.borderWidth));
	QRectF backgroundRect = rect();
	backgroundRect.adjust(
		t.metrics.borderWidth / 2.0,
		t.metrics.borderWidth / 2.0,
		-t.metrics.borderWidth / 2.0,
		-t.metrics.borderWidth / 2.0
	);
	//painter.drawRoundedRect(backgroundRect, t.radii.small, t.radii.small);
	painter.drawRect(backgroundRect);

	//draw name
	painter.setFont(t.fonts.button);
	painter.setPen(t.colors.textPrimary);
	const QRect nameRect = rect().adjusted(height(), 8, -t.metrics.spacingSm, -height() / 2);
	painter.drawText(
		nameRect,
		Qt::AlignLeft | Qt::AlignVCenter,
		QFontMetrics(t.fonts.button).elidedText(displayName, Qt::ElideRight, nameRect.width())
	);

	//draw post
	painter.setFont(t.fonts.small);
	painter.setPen(t.colors.textSecondary);
	const QRect postRect = rect().adjusted(height(), height() / 2, -t.metrics.spacingSm, -6);
	painter.drawText(
		postRect,
		Qt::AlignLeft | Qt::AlignVCenter,
		QFontMetrics(t.fonts.small).elidedText(post, Qt::ElideRight, postRect.width())
	);

	//draw round avatar
	const int avatarExtent = qRound(height() * 0.75);
	const QRect avatarRect(8, (height() - avatarExtent) / 2, avatarExtent, avatarExtent);
	QPixmap avatarPixmap = QPixmap::fromImage(avatar.scaled(avatarExtent, avatarExtent, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
	QPainterPath path;
	path.addEllipse(avatarRect);
	painter.setClipPath(path);
	painter.drawPixmap(avatarRect, avatarPixmap);
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
	Q_UNUSED(e)
	pressed = false;
	update();
}
