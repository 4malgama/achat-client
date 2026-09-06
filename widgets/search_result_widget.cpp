#include "search_result_widget.h"
#include "../utils/image_utils.h"
#include <QPainter>
#include <QMouseEvent>


SearchResultWidget::SearchResultWidget(QWidget *parent)
	: ThemedWidget{parent}
{
	connect(this, SIGNAL(pixmapChanged()), this, SLOT(onSomeChanged()));
	connect(this, SIGNAL(loginChanged()), this, SLOT(onSomeChanged()));
	connect(this, SIGNAL(displayNameChanged()), this, SLOT(onSomeChanged()));
	setFixedHeight(80);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Ignored);
	setCursor(Qt::PointingHandCursor);
	onThemeChanged(theme());
}

SearchResultWidget::SearchResultWidget(QWidget *parent, const QPixmap &pixmap, const QString &login, const QString &displayName)
	: ThemedWidget{parent}
{
	this->pixmap = pixmap;
	this->login = login;
	this->displayName = displayName;

	connect(this, SIGNAL(pixmapChanged()), this, SLOT(onSomeChanged()));
	connect(this, SIGNAL(loginChanged()), this, SLOT(onSomeChanged()));
	connect(this, SIGNAL(displayNameChanged()), this, SLOT(onSomeChanged()));
	setFixedHeight(80);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Ignored);
	setCursor(Qt::PointingHandCursor);
	onThemeChanged(theme());
}

void SearchResultWidget::paintEvent(QPaintEvent *)
{
	const ThemeData& t = theme();

	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	int offset = height();

	QColor bgColor = pressed
		? t.colors.surfacePressed
		: (hovered ? t.colors.surfaceHover : t.colors.surfaceBackground);

	painter.setPen(QPen(t.colors.border, t.metrics.borderWidth));
	painter.setBrush(bgColor);
	QRectF backgroundRect = rect();
	backgroundRect.adjust(
		t.metrics.borderWidth / 2.0,
		t.metrics.borderWidth / 2.0,
		-t.metrics.borderWidth / 2.0,
		-t.metrics.borderWidth / 2.0
	);
	painter.drawRoundedRect(backgroundRect, t.radii.medium, t.radii.medium);

	//displayName
	painter.setFont(t.fonts.button);
	painter.setPen(t.colors.textPrimary);
	const QRect displayNameRect = rect().adjusted(offset, t.metrics.spacingSm, -t.metrics.spacingMd, -height() / 2);
	painter.drawText(
		displayNameRect,
		Qt::AlignLeft | Qt::AlignVCenter,
		QFontMetrics(t.fonts.button).elidedText(displayName, Qt::ElideRight, displayNameRect.width())
	);

	//login
	painter.setFont(t.fonts.small);
	painter.setPen(t.colors.textSecondary);
	const QRect loginRect = rect().adjusted(offset, height() / 2, -t.metrics.spacingMd, -t.metrics.spacingSm);
	painter.drawText(
		loginRect,
		Qt::AlignLeft | Qt::AlignVCenter,
		QFontMetrics(t.fonts.small).elidedText(login, Qt::ElideRight, loginRect.width())
	);

	//avatar
	QRect avatarRect(0, 0, offset, offset);
	avatarRect.adjust(10, 10, -10, -10);
	painter.drawImage(avatarRect, ImageUtils::makeCircularAvatar(pixmap.toImage(), avatarRect.width()));
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
	{
		if (pressed)
			emit clicked();
		pressed = false;
	}
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

void SearchResultWidget::onThemeChanged(const ThemeData &theme)
{
	setFont(theme.fonts.base);
	ThemedWidget::onThemeChanged(theme);
}
