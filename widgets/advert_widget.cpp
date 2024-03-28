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
	: QWidget{parent}
{
	setLayout(new QHBoxLayout(this));
	setCursor(Qt::PointingHandCursor);
}

AdvertWidget::AdvertWidget(QColor a, QColor b, QWidget *parent)
	: AdvertWidget{parent}
{
	background.a = a;
	background.b = b;
	setCursor(Qt::PointingHandCursor);
}

void AdvertWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	QLinearGradient gradient(rect().topLeft(), rect().topRight());
	gradient.setColorAt(0, background.a);
	gradient.setColorAt(1, background.b);
	painter.fillRect(rect(), gradient);

	painter.setPen(Qt::white);

	QFont titleFont("Segoe UI", 20, QFont::Bold);
	QFont descriptionFont("Segoe UI", 12, QFont::Normal);

	painter.setFont(titleFont);
	painter.drawText(rect(), Qt::AlignHCenter | Qt::AlignTop, title);

	painter.setFont(descriptionFont);
	painter.drawText(rect().adjusted(0, 30 , 0, 0), Qt::AlignHCenter | Qt::AlignTop, description);

	//draw button
	if (linkText.isEmpty() == false)
	{
		QPen pen;
		pen.setColor(Qt::white);
		pen.setWidth(2);
		painter.setPen(pen);

		QFontMetrics fm(descriptionFont);
		QString elidedText = fm.elidedText(linkText, Qt::ElideMiddle, rect().width() - 20);
		int buttonHeight = 30;
		int buttonWidth = fm.horizontalAdvance(elidedText) + 20;
		QRect buttonRect = QRect(
			(rect().width() - buttonWidth) / 2,
			60,
			buttonWidth,
			buttonHeight
		);
		QPainterPath path;
		path.addRoundedRect(buttonRect, 10, 10);
		//painter.fillPath(path, Qt::white);
		painter.drawText(buttonRect, Qt::AlignCenter, elidedText);
		painter.drawPath(path.translated(0, 2));
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
}

void AdvertWidget::setDescription(const QString& description)
{
	this->description = description;
}

void AdvertWidget::setLink(const QString& link)
{
	this->link = link;
}

void AdvertWidget::setLinkText(const QString& linkText)
{
	this->linkText = linkText;
}

void AdvertWidget::setImage(const QImage& image)
{
	this->image = image;
}