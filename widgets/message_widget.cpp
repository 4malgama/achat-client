#include "message_widget.h"
#include "../client.h"

#include <QPushButton>
#include <QPainter>
#include <QVariant>

MessageWidget::MessageWidget(Client *client)
	: ThemedWidget{client}
{
	if (client == nullptr)
		throw std::invalid_argument("client can't be nullptr");
	icon = UNKNOWN;
	btnClose = new QPushButton(this);
	btnClose->setText(tr("Close"));
	btnClose->setCursor(Qt::CursorShape::PointingHandCursor);
	btnClose->setProperty("role", "primary");

	setWindowFlags(Qt::WindowType::FramelessWindowHint);
	setAttribute(Qt::WidgetAttribute::WA_TranslucentBackground);
	resize(400, 200);
	setMinimumSize(400, 200);
	setMaximumSize(400, 200);
	btnClose->setGeometry(width() / 2 - 50, height() - 50, 100, 30);
	setWindowTitle(tr("Message"));
	move(client->width() / 2 - width() / 2, client->height() / 2 - height() / 2);

	connect(client, &Client::event_resize, this, [this] (QSize size) {
		move(size.width() / 2 - width() / 2, size.height() / 2 - height() / 2);
	});
	connect(btnClose, &QPushButton::pressed, this, &MessageWidget::close);

	onThemeChanged(theme());
}

void MessageWidget::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event)

	const ThemeData& t = theme();

	QPainter painter(this);
	painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing | QPainter::Antialiasing);

	QColor tone = (icon == MessageWidget::INFO) ?
		t.colors.accent
		: (icon == MessageWidget::WARNING) ?
			t.colors.warning
			: (icon == MessageWidget::ERROR) ?
				t.colors.danger
				: t.colors.surfaceBackground;

	QRectF backgroundRect = rect();
	backgroundRect.adjust(
		t.metrics.borderWidth / 2.0,
		t.metrics.borderWidth / 2.0,
		-t.metrics.borderWidth / 2.0,
		-t.metrics.borderWidth / 2.0
	);
	QPen pen(tone, t.metrics.borderWidth);
	painter.setPen(pen);
	painter.setBrush(t.colors.panelBackground);
	painter.drawRoundedRect(backgroundRect, t.radii.large, t.radii.large);

	painter.setPen(Qt::NoPen);
	painter.setBrush(tone);
	painter.drawRoundedRect(QRectF(0, 0, width(), 34), t.radii.large, t.radii.large);
	painter.fillRect(QRectF(0, 20, width(), 14), tone);

	painter.setPen(t.colors.textOnAccent);
	painter.setFont(t.fonts.button);
	painter.drawText(QRect(12, 0, width() - 24, 34), Qt::AlignCenter, windowTitle());

	painter.setPen(t.colors.textPrimary);
	painter.setFont(t.fonts.base);
	painter.drawText(
		QRect(20, 50, width() - 40, height() - 110),
		Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap,
		message
	);
}

void MessageWidget::onThemeChanged(const ThemeData &theme)
{
	setFont(theme.fonts.base);
	btnClose->setFont(theme.fonts.button);
	btnClose->setGeometry(
		width() / 2 - 55,
		height() - theme.metrics.smallControlHeight - theme.metrics.spacingLg,
		110,
		theme.metrics.smallControlHeight
	);
	updateGeometry();
	update();
}
