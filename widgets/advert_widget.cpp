#include "advert_widget.h"

#include <QHBoxLayout>
#include <QPainter>
#include <QFont>
#include <QPainterPath>
#include <QBrush>
#include <QPen>
#include <QLinearGradient>
#include <QMouseEvent>
#include <QDesktopServices>



AdvertWidget::AdvertWidget(QWidget *parent)
	: ThemedWidget{parent}
{
	setLayout(new QHBoxLayout(this));
	setCursor(Qt::PointingHandCursor);

	anim = new QPropertyAnimation(this, "offset");
	anim->setStartValue(0.0);
	anim->setKeyValueAt(0.5, 1.0);
	anim->setEndValue(0.0);
	anim->setDuration(4000);
	anim->setLoopCount(-1);
	anim->setEasingCurve(QEasingCurve::InOutSine);
	anim->start();

	onThemeChanged(theme());
}

void AdvertWidget::paintEvent(QPaintEvent *)
{
	const ThemeData& t = theme();

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	QLinearGradient gradient(
			rect().topLeft() + QPointF(rect().width() * m_offset, 0),
			rect().bottomRight() + QPointF(rect().width() * m_offset, 0)
	);

	gradient.setColorAt(0.0, t.colors.accentPressed);
	gradient.setColorAt(1.0, t.colors.accentHover);

	QRectF backgroundRect = rect();
	backgroundRect.adjust(
		t.metrics.borderWidth / 2.0,
		t.metrics.borderWidth / 2.0,
		-t.metrics.borderWidth / 2.0,
		-t.metrics.borderWidth / 2.0
	);
	painter.setPen(QPen(t.colors.borderStrong, t.metrics.borderWidth));
	painter.setBrush(gradient);
	painter.drawRoundedRect(backgroundRect, t.radii.medium, t.radii.medium);

	painter.setPen(t.colors.textOnAccent);

	QFont titleFont = t.fonts.title;
	QFont descriptionFont = t.fonts.base;

	painter.setFont(titleFont);
	painter.drawText(rect(), Qt::AlignHCenter | Qt::AlignTop, title);

	painter.setFont(descriptionFont);
	painter.drawText(rect().adjusted(0, 30 , 0, 0), Qt::AlignHCenter | Qt::AlignTop, description);

	//draw button
	if (linkText.isEmpty() == false)
	{
		QPen pen;
		pen.setColor(t.colors.textOnAccent);
		pen.setWidth(t.metrics.borderWidth);
		painter.setPen(pen);

		painter.setFont(t.fonts.button);
		QFontMetrics fm(t.fonts.button);
		QString elidedText = fm.elidedText(linkText, Qt::ElideMiddle, rect().width() - 20);
		int buttonHeight = t.metrics.smallControlHeight;
		int buttonWidth = fm.horizontalAdvance(elidedText) + 20;
		QRect buttonRect = QRect(
			(rect().width() - buttonWidth) / 2,
			60,
			buttonWidth,
			buttonHeight
		);
		QPainterPath path;
		path.addRoundedRect(buttonRect, t.radii.medium, t.radii.medium);
		QColor buttonBackground = t.colors.panelBackground;
		buttonBackground.setAlpha(90);
		painter.fillPath(path, buttonBackground);
		painter.drawText(buttonRect, Qt::AlignCenter, elidedText);
		painter.drawPath(path);
	}
}

void AdvertWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() != Qt::MouseButton::LeftButton)
		return;

	if (link.isEmpty() == false)
	{
		QDesktopServices::openUrl(QUrl(link));
	}
}

void AdvertWidget::setTitle(const QString& title)
{
	this->title = title;
	update();
}

void AdvertWidget::setDescription(const QString& description)
{
	this->description = description;
	update();
}

void AdvertWidget::setLink(const QString& link)
{
	this->link = link;
	update();
}

void AdvertWidget::setLinkText(const QString& linkText)
{
	this->linkText = linkText;
	update();
}

void AdvertWidget::setImage(const QImage& image)
{
	this->image = image;
	update();
}


qreal AdvertWidget::offset() const
{
	return m_offset;
}

void AdvertWidget::setOffset(qreal newOffset)
{
	if (qFuzzyCompare(m_offset, newOffset))
		return;
	m_offset = newOffset;
	emit offsetChanged();
	update();
}

void AdvertWidget::onThemeChanged(const ThemeData &theme)
{
	setFont(theme.fonts.base);
	updateGeometry();
	update();
}
