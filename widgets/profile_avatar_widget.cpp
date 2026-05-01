#include "profile_avatar_widget.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>


ProfileAvatarWidget::ProfileAvatarWidget(QWidget *parent)
	: QWidget{parent}
{
	setFixedSize(200, 200);
	setCursor(Qt::PointingHandCursor);
	setToolTip(tr("Change profile photo"));
}

QImage ProfileAvatarWidget::image() const
{
	return m_image;
}

void ProfileAvatarWidget::setImage(const QImage &newImage)
{
	if (m_image == newImage)
		return;
	m_image = newImage;
	emit imageChanged();
	update();
}

void ProfileAvatarWidget::paintEvent(QPaintEvent *)
{
	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing, true);
	p.setRenderHint(QPainter::SmoothPixmapTransform, true);

	const qreal dpr = devicePixelRatioF();

	QImage canvas(size() * dpr, QImage::Format_ARGB32_Premultiplied);
	canvas.setDevicePixelRatio(dpr);
	canvas.fill(Qt::transparent);

	QPainter cp(&canvas);
	cp.setRenderHint(QPainter::Antialiasing, true);
	cp.setRenderHint(QPainter::SmoothPixmapTransform, true);

	QRectF r = rect();
	r.adjust(1.0, 1.0, -1.0, -1.0);

	QPainterPath path;
	path.addEllipse(r);

	cp.setClipPath(path);

	QImage scaled = m_image.scaled(
		size() * dpr,
		Qt::KeepAspectRatioByExpanding,
		Qt::SmoothTransformation
	);
	scaled.setDevicePixelRatio(dpr);

	const QPointF pos(
		(width() - scaled.width() / dpr) / 2.0,
		(height() - scaled.height() / dpr) / 2.0
	);

	cp.drawImage(pos, scaled);
	cp.end();

	p.drawImage(0, 0, canvas);
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
