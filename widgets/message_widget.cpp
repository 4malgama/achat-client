#include "message_widget.h"

#include <QPushButton>
#include <QPainter>

MessageWidget::MessageWidget(Client *client)
	: QWidget{client}
{
	if (client == nullptr)
		throw std::invalid_argument("client can't be nullptr");
	btnClose = new QPushButton(this);
	btnClose->setText(tr("Close"));
	btnClose->setCursor(Qt::CursorShape::PointingHandCursor);
	
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
}

void MessageWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing | QPainter::Antialiasing);

	QColor tone = (icon == MessageWidget::INFO) ?
		QColor(18, 70, 108)
		: (icon == MessageWidget::WARNING) ?
			QColor(170, 110, 0)
			: (icon == MessageWidget::ERROR) ?
				QColor(170, 40, 40)
				: QColor(50, 50, 50);
	
	//background
	QColor backgroundColor(tone);
	QBrush brush(backgroundColor);
	QPen pen(Qt::white);
	pen.setWidth(2);
	painter.setPen(pen);
	painter.setBrush(brush);
	painter.drawRoundedRect(rect(), 10, 10);

	//titlebar
	QColor titleBarColor(tone);
	titleBarColor = titleBarColor.lighter(200);
	titleBarColor.setAlpha(200);
	QBrush titleBarBrush(titleBarColor);
	QPen titleBarPen(Qt::white);
	titleBarPen.setWidth(1);
	painter.setPen(titleBarPen);
	painter.setBrush(titleBarBrush);
	painter.drawRoundedRect(0, 0, width(), 30, 10, 10);

	//title
	QFont titleFont("Segoe UI", 14, QFont::Bold);
	painter.setFont(titleFont);
	painter.drawText(rect(), Qt::AlignHCenter | Qt::AlignTop, windowTitle());

	//message
	QFont messageFont("Segoe UI", 12, QFont::Normal);
	painter.setFont(messageFont);
	painter.drawText(rect().adjusted(0, 40, 0, 0), Qt::AlignHCenter | Qt::AlignTop, message);

	//buttons
}