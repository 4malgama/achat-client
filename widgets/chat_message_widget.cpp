#include "chat_message_widget.h"

#include <QPainter>


ChatMessageWidget::ChatMessageWidget(QWidget *parent, bool isMine)
	: QWidget{parent}
{
	m_Colors.text = QColor(255, 255, 255);
	m_Colors.background = isMine ? QColor(59, 104, 145) : QColor(29, 52, 73);
	m_Colors.date = QColor(255, 255, 255, 130);
	m_Mine = isMine;

	connect(this, &ChatMessageWidget::textChanged, this, &ChatMessageWidget::onTextChanged);
	connect(this, &ChatMessageWidget::dateChanged, this, &ChatMessageWidget::onDateChanged);
}

void ChatMessageWidget::setText(const QString &t)
{
	m_Text = t;
	emit textChanged();
}

void ChatMessageWidget::setDateTime(const QDateTime &t)
{
	m_DateTime = t;
	emit dateChanged();
}

QString ChatMessageWidget::text() const
{
	return m_Text;
}

QDateTime ChatMessageWidget::dateTime() const
{
	return m_DateTime;
}

bool ChatMessageWidget::isMine() const
{
	return m_Mine;
}

void ChatMessageWidget::onDateChanged()
{
	update();
}

void ChatMessageWidget::onTextChanged()
{
	QFont font("Segoe UI", 12, QFont::Normal);

	QFontMetrics fontMetrics(font);
	int width = fontMetrics.boundingRect(m_Text).width();
	int height = fontMetrics.boundingRect(m_Text).height();
	setFixedSize(width + 100, height + 30);

	update();
}

void ChatMessageWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);

	//draw bubble
	painter.setPen(Qt::NoPen);
	painter.setBrush(m_Colors.background);
	painter.drawRoundedRect(rect(), 10, 10);

	//draw text
	QFont font("Segoe UI", 12, QFont::Normal);
	painter.setFont(font);
	painter.setPen(m_Colors.text);
	painter.drawText(rect().adjusted(10, 5, -5, -10), Qt::AlignLeft | Qt::TextWordWrap, m_Text);

	//draw datetime
	QFont dateTimeFont("Segoe UI", 8, QFont::Normal);
	painter.setFont(dateTimeFont);
	painter.setPen(m_Colors.date);
	painter.drawText(rect().adjusted(10, 10, -5, -10), Qt::AlignLeft | Qt::AlignBottom, m_DateTime.toString("dd.MM.yyyy hh:mm"));
}
