#include "profile_avatar_widget.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>


ProfileAvatarWidget::ProfileAvatarWidget(QWidget *parent)
	: ThemedWidget{parent}
{
	setCursor(Qt::PointingHandCursor);
	setToolTip(tr("Change profile photo"));
	onThemeChanged(theme());
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
	const ThemeData& t = theme();
	const qreal borderInset = t.metrics.avatarBorderWidth / 2.0;
	r.adjust(borderInset, borderInset, -borderInset, -borderInset);

	QPainterPath path;
	path.addEllipse(r);

	cp.setClipPath(path);

	if (!m_image.isNull())
	{
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
	}
	else
	{
		cp.fillPath(path, t.colors.surfaceBackground);
	}

	if (m_isHovered)
		cp.fillPath(path, t.colors.avatarHoverOverlay);
	cp.end();

	p.drawImage(0, 0, canvas);
	p.setPen(QPen(t.colors.avatarBorder, t.metrics.avatarBorderWidth));
	p.setBrush(Qt::NoBrush);
	p.drawEllipse(r);
}

void ProfileAvatarWidget::mousePressEvent(QMouseEvent *e)
{
	if (e->buttons() & Qt::LeftButton)
		emit clicked();
}

void ProfileAvatarWidget::enterEvent(QEvent *)
{
	m_isHovered = true;
	update();
}

void ProfileAvatarWidget::leaveEvent(QEvent *)
{
	m_isHovered = false;
	update();
}

void ProfileAvatarWidget::onThemeChanged(const ThemeData &theme)
{
	setFixedSize(theme.metrics.avatarSize, theme.metrics.avatarSize);
	setFont(theme.fonts.base);
	ThemedWidget::onThemeChanged(theme);
}
