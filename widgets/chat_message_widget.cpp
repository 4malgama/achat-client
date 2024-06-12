#include "chat_message_widget.h"
#include "attachment_button_widget.h"
#include "../client.h"
#include <QPainter>


namespace client
{
	extern Client* window;
}

ChatMessageWidget::ChatMessageWidget(QWidget *parent, bool isMine)
	: QWidget{parent}
	, ATTACHMENT_HEIGHT(30)
{
	m_Colors.text = QColor(255, 255, 255);
	m_Colors.background = isMine ? QColor(59, 104, 145) : QColor(29, 52, 73);
	m_Colors.date = QColor(255, 255, 255, 130);
	m_Mine = isMine;

	setContextMenuPolicy(Qt::CustomContextMenu);

	connect(m_Menu.addAction(tr("Reply")), &QAction::triggered, this, &ChatMessageWidget::onReply);
	connect(m_Menu.addAction(tr("Forward")), &QAction::triggered, this, &ChatMessageWidget::onForward);
	connect(m_Menu.addAction(tr("Copy text")), &QAction::triggered, this, &ChatMessageWidget::onCopyText);
	connect(m_Menu.addAction(tr("Delete")), &QAction::triggered, this, &ChatMessageWidget::onDelete);

	connect(this, &ChatMessageWidget::textChanged, this, &ChatMessageWidget::onTextChanged);
	connect(this, &ChatMessageWidget::dateChanged, this, &ChatMessageWidget::onDateChanged);
	connect(this, &ChatMessageWidget::attachmentsChanged, this, &ChatMessageWidget::onAttachmentsChanged);
	connect(this, &QWidget::customContextMenuRequested, this, &ChatMessageWidget::onMenuCalled);
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

void ChatMessageWidget::onAttachmentsChanged()
{
	int i = 0;
	QFont defaultFont("Segoe UI", 12, QFont::Normal);
	QFontMetrics fontMetrics(defaultFont);
	int skipHeight = fontMetrics.boundingRect(m_Text).height();
	for (const ChatMessageAttachment& a : m_Attachments)
	{
		AttachmentButtonWidget* btn = new AttachmentButtonWidget(this);
		btn->setFileName(a.name);
		btn->setFileSize(a.size / 1024.0);
		btn->setPixmap(QPixmap(":/r/resources/images/file.png"));
		btn->setFixedSize(width() - 20, 30);
		btn->move(10, ATTACHMENT_HEIGHT * i + skipHeight + 5);
		btn->show();
		i++;
	}

	onTextChanged();
}

void ChatMessageWidget::onMenuCalled(const QPoint&)
{
	QPoint pos = QCursor::pos();
	QSize size = m_Menu.sizeHint();

	if (size.width() + pos.x() > client::window->pos().x() + client::window->size().width())
	{
		pos.setX(pos.x() - size.width());
		m_Menu.toRight = false;
	}
	else
	{
		pos.setX(pos.x());
		m_Menu.toRight = true;
	}
	pos.setY(pos.y());
	m_Menu.move(pos);
	m_Menu.exec();
}

void ChatMessageWidget::onReply()
{

}

void ChatMessageWidget::onForward()
{

}

void ChatMessageWidget::onCopyText()
{

}

void ChatMessageWidget::onDelete()
{

}

QList<ChatMessageAttachment> ChatMessageWidget::attachments() const
{
	return m_Attachments;
}

void ChatMessageWidget::setAttachments(const QList<ChatMessageAttachment> &newAttachments)
{
	m_Attachments = newAttachments;
	emit attachmentsChanged();
}

void ChatMessageWidget::onTextChanged()
{
	QFont font("Segoe UI", 12, QFont::Normal);

	QFontMetrics fontMetrics(font);

	const int attachmentHeight = m_Attachments.size() * ATTACHMENT_HEIGHT;

	//setFixedSize(fontMetrics.boundingRect(m_Text).width() + 100, fontMetrics.boundingRect(m_Text).height() + 30 + attachmentHeight);
	int maxTextWidth = static_cast<int>(client::window->width() * 0.4f);
	QRect boundingRect = fontMetrics.boundingRect(QRect(0, 0, maxTextWidth, 0), Qt::TextWordWrap, m_Text);

	setFixedSize(boundingRect.width() + 100, boundingRect.height() + 30 + attachmentHeight);

	update();
}

void ChatMessageWidget::paintEvent(QPaintEvent *)
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
