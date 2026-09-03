#include "chat_message_widget.h"
#include "attachment_button_widget.h"
#include "../client.h"
#include "../network/account.h"
#include <algorithm>
#include <initializer_list>
#include <QPainter>


namespace client
{
	extern Client* window;
}

ChatMessageWidget::ChatMessageWidget(QWidget *parent, bool isMine)
	: ThemedWidget{parent}
	, m_Mine(isMine)
{
	setContextMenuPolicy(Qt::CustomContextMenu);

	connect(m_Menu.addAction(tr("Reply")), &QAction::triggered, this, &ChatMessageWidget::onReply);
	connect(m_Menu.addAction(tr("Forward")), &QAction::triggered, this, &ChatMessageWidget::onForward);
	connect(m_Menu.addAction(tr("Copy text")), &QAction::triggered, this, &ChatMessageWidget::onCopyText);
	connect(m_Menu.addAction(tr("Delete")), &QAction::triggered, this, &ChatMessageWidget::onDelete);

	connect(this, &ChatMessageWidget::textChanged, this, &ChatMessageWidget::onTextChanged);
	connect(this, &ChatMessageWidget::dateChanged, this, &ChatMessageWidget::onDateChanged);
	connect(this, &ChatMessageWidget::attachmentsChanged, this, &ChatMessageWidget::onAttachmentsChanged);
	connect(this, &QWidget::customContextMenuRequested, this, &ChatMessageWidget::onMenuCalled);

	onThemeChanged(theme());
}

void ChatMessageWidget::setText(const QString &t)
{
	if (m_Text == t)
		return;

	m_Text = t;
	emit textChanged();
}

void ChatMessageWidget::setDateTime(const QDateTime &t)
{
	if (m_DateTime == t)
		return;

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
	rebuildLayout();
	//update();
}

void ChatMessageWidget::onAttachmentsChanged()
{
	// int i = 0;
	// QFontMetrics fontMetrics(font());
	// int skipHeight = fontMetrics.boundingRect(m_Text).height();
	// for (const ChatMessageAttachment& a : m_Attachments)
	// {
	// 	AttachmentButtonWidget* btn = new AttachmentButtonWidget(this);
	// 	uint64 id = a.id;
	// 	connect(btn, &QAbstractButton::clicked, this, [this, id] { onDownloadClicked(id); });
	// 	btn->setFileName(a.name);
	// 	btn->setFileSize(a.size / 1024.0);
	// 	btn->setPixmap(QPixmap(":/r/resources/images/file.png"));
	// 	btn->setFixedSize(width() - 20, 30);
	// 	btn->move(10, ATTACHMENT_HEIGHT * i + skipHeight + 5);
	// 	btn->show();
	// 	i++;
	// }

	// onTextChanged();
	rebuildAttachmentButtons();
	rebuildLayout();
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
	rebuildLayout();
}

void ChatMessageWidget::paintEvent(QPaintEvent *)
{
	const ThemeData& t = theme();

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

	QRectF bubbleRect = rect();
	bubbleRect.adjust(
		t.metrics.borderWidth / 2.0,
		t.metrics.borderWidth / 2.0,
		-t.metrics.borderWidth / 2.0,
		-t.metrics.borderWidth / 2.0
	);

	painter.setPen(QPen(t.colors.border, t.metrics.borderWidth));
	painter.setBrush(isMine() ? t.colors.accent : t.colors.cardBackground);
	painter.drawRoundedRect(bubbleRect, t.radii.large, t.radii.large);

	const int paddingX = t.metrics.chatBubblePaddingX;
	const int paddingY = t.metrics.chatBubblePaddingY;
	const int spacing = t.metrics.chatBubbleSpacing;
	const int dateHeight = t.metrics.chatBubbleDateHeight;

	const QRect textArea(
		paddingX,
		paddingY,
		width() - paddingX * 2,
		height() - paddingY * 2 - dateHeight - spacing
	);

	painter.setFont(t.fonts.message);

	painter.setPen(t.colors.textPrimary);

	painter.drawText(
		textArea,
		Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap,
		m_Text
	);

	const QRect dateArea(
		paddingX,
		height() - paddingY - dateHeight,
		width() - paddingX * 2,
		dateHeight
	);

	painter.setFont(t.fonts.messageDate);
	painter.setPen(isMine() ? QColor(255, 255, 255, 180) : t.colors.textSecondary);

	painter.drawText(
		dateArea,
		Qt::AlignRight | Qt::AlignVCenter,
		m_DateTime.toString("dd.MM.yyyy hh:mm")
	);
}

void ChatMessageWidget::onThemeChanged(const ThemeData &theme)
{
	setFont(theme.fonts.base);

	for (AttachmentButtonWidget* btn : m_AttachmentButtons)
		btn->setFont(theme.fonts.button);

	rebuildLayout();

	updateGeometry();
	update();
}

void ChatMessageWidget::onDownloadClicked(uint64 id)
{
	client::window->acc->downloadFile(id);
}

int ChatMessageWidget::maxBubbleWidth()
{
	const ThemeData& t = theme();

	QWidget* root = window();
	const int baseWidth = root ? root->width() : 800;

	return static_cast<int>(baseWidth * (t.metrics.chatBubbleMaxWidthPercent / 100.0));
}

void ChatMessageWidget::rebuildAttachmentButtons()
{
	qDeleteAll(m_AttachmentButtons);
	m_AttachmentButtons.clear();

	for (const ChatMessageAttachment& a : m_Attachments)
	{
		auto* btn = new AttachmentButtonWidget(this);

		const uint64 id = a.id;

		connect(btn, &QAbstractButton::clicked, this, [this, id] {
			onDownloadClicked(id);
		});

		btn->setFileName(a.name);
		btn->setFileSize(a.size / 1024.0);
		btn->setPixmap(QPixmap(":/r/resources/images/file.png"));
		btn->show();

		m_AttachmentButtons.append(btn);
	}
}

void ChatMessageWidget::rebuildLayout()
{
	const ThemeData& t = theme();

	const int paddingX = t.metrics.chatBubblePaddingX;
	const int paddingY = t.metrics.chatBubblePaddingY;
	const int spacing = t.metrics.chatBubbleSpacing;
	const int dateHeight = t.metrics.chatBubbleDateHeight;
	const int attachmentButtonHeight = t.metrics.attachmentButtonHeight;

	const int maxWidth = maxBubbleWidth();
	const int contentMaxWidth = qMax(50, maxWidth - paddingX * 2);

	QFontMetrics textFm(t.fonts.message);
	QFontMetrics dateFm(t.fonts.messageDate);

	const QRect textRect = textFm.boundingRect(
		QRect(0, 0, contentMaxWidth, 0),
		Qt::TextWordWrap,
		m_Text
	);

	const QString dateString = m_DateTime.toString("dd.MM.yyyy hh:mm");
	const int dateWidth = dateFm.horizontalAdvance(dateString);

	int attachmentsHeight = 0;
	if (!m_Attachments.isEmpty())
	{
		attachmentsHeight =
			m_Attachments.size() * attachmentButtonHeight +
			(m_Attachments.size() - 1) * spacing;
	}

	const int contentWidth = std::max({
		textRect.width(),
		dateWidth,
		m_Attachments.isEmpty() ? 0 : contentMaxWidth,
		t.metrics.chatBubbleMinWidth
	});

	int totalHeight = paddingY;
	totalHeight += textRect.height();

	if (!m_Attachments.isEmpty())
	{
		totalHeight += spacing;
		totalHeight += attachmentsHeight;
	}

	totalHeight += spacing;
	totalHeight += dateHeight;
	totalHeight += paddingY;

	const int totalWidth = qMin(maxWidth, contentWidth + paddingX * 2);

	setFixedSize(totalWidth, totalHeight);

	int y = paddingY + textRect.height();

	if (!m_Attachments.isEmpty())
		y += spacing;

	for (AttachmentButtonWidget* btn : m_AttachmentButtons)
	{
		btn->setFixedSize(width() - paddingX * 2, attachmentButtonHeight);
		btn->move(paddingX, y);
		y += attachmentButtonHeight + spacing;
	}

	updateGeometry();
	update();
}
