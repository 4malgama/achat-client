#include "search_result_widget.h"
#include "../utils/image_utils.h"
#include <QPainter>
#include <QMouseEvent>


SearchResultWidget::SearchResultWidget(QWidget *parent)
	: QWidget{parent}
	, font("Segoe UI", 14)
{
	connect(this, SIGNAL(pixmapChanged()), this, SLOT(onSomeChanged()));
	connect(this, SIGNAL(loginChanged()), this, SLOT(onSomeChanged()));
	connect(this, SIGNAL(displayNameChanged()), this, SLOT(onSomeChanged()));
	setFixedHeight(80);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Ignored);
}

SearchResultWidget::SearchResultWidget(QWidget *parent, const QPixmap &pixmap, const QString &login, const QString &displayName)
	: QWidget{parent}
	, font("Segoe UI", 14)
{
	this->pixmap = pixmap;
	this->login = login;
	this->displayName = displayName;

	connect(this, SIGNAL(pixmapChanged()), this, SLOT(onSomeChanged()));
	connect(this, SIGNAL(loginChanged()), this, SLOT(onSomeChanged()));
	connect(this, SIGNAL(displayNameChanged()), this, SLOT(onSomeChanged()));
	setFixedHeight(80);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Ignored);
}

void SearchResultWidget::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.setFont(font);
	int offset = height();

	//background
	QColor bgColor = (pressed ? colors.bg.pressed : (hovered ? colors.bg.hovered : colors.bg.common));

	painter.setPen(Qt::NoPen);
	painter.setBrush(bgColor);

	painter.drawRoundedRect(rect(), 10, 10);

	//displayName
	painter.setPen(Qt::white);

	painter.drawText(rect().adjusted(offset, 10, -10, -10), displayName, Qt::AlignLeft | Qt::AlignTop);

	//login
	painter.setPen(Qt::gray);

	painter.drawText(rect().adjusted(offset, 10, -10, -10), login, Qt::AlignLeft | Qt::AlignBottom);

	//avatar
	QRect avatarRect(0, 0, offset, offset);
	avatarRect.adjust(10, 10, -10, -10);
	painter.drawImage(avatarRect, ImageUtils::CropImageToCircle(pixmap.toImage(), avatarRect.width()));
}

void SearchResultWidget::enterEvent(QEvent *)
{
	hovered = true;
	update();
}

void SearchResultWidget::leaveEvent(QEvent *)
{
	hovered = false;
	update();
}

void SearchResultWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
		pressed = true;
	update();
}

void SearchResultWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
		pressed = false;
	update();
}

QString SearchResultWidget::getDisplayName() const
{
	return displayName;
}

void SearchResultWidget::setDisplayName(const QString &newDisplayName)
{
	if (displayName == newDisplayName)
		return;
	displayName = newDisplayName;
	emit displayNameChanged();
}

void SearchResultWidget::onSomeChanged()
{
	update();
}

QString SearchResultWidget::getLogin() const
{
	return login;
}

void SearchResultWidget::setLogin(const QString &newLogin)
{
	if (login == newLogin)
		return;
	login = newLogin;
	emit loginChanged();
}

QPixmap SearchResultWidget::getPixmap() const
{
	return pixmap;
}

void SearchResultWidget::setPixmap(const QPixmap &newPixmap)
{
	if (pixmap == newPixmap)
		return;
	pixmap = newPixmap;
	emit pixmapChanged();
}
