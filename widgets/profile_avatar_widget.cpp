#include "profile_avatar_widget.h"

#include <QMouseEvent>
#include <QPainter>


ProfileAvatarWidget::ProfileAvatarWidget(QWidget *parent)
	: QWidget{parent}
{
	setFixedSize(200, 200);
}

QImage ProfileAvatarWidget::image() const
{
	return m_image;
}

void ProfileAvatarWidget::setImage(const QImage &newImage)
{
	if (m_image == newImage)
		return;
	m_image = newImage.scaled(width(), height(), Qt::KeepAspectRatioByExpanding);
	emit imageChanged();
	update();
}

void ProfileAvatarWidget::paintEvent(QPaintEvent *)
{
	QPainter p(this);
	p.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);

	p.setPen(Qt::NoPen);

	p.drawImage(0, 0, m_image);
}

void ProfileAvatarWidget::mousePressEvent(QMouseEvent *e)
{
	if (e->buttons() & Qt::LeftButton)
		emit clicked();
}

void ProfileAvatarWidget::enterEvent(QEvent *)
{
	m_isHovered = true;
}

void ProfileAvatarWidget::leaveEvent(QEvent *)
{
	m_isHovered = false;
}
