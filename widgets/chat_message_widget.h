#ifndef CHATMESSAGEWIDGET_H
#define CHATMESSAGEWIDGET_H

#include "../types/classes.h"
#include "dropdown_menu.h"
#include "themed_widget.h"
#include <QDateTime>

class AttachmentButtonWidget;

class ChatMessageWidget : public ThemedWidget
{
	Q_OBJECT

public:
	explicit ChatMessageWidget(QWidget *parent = nullptr, bool isMine = true);

	void setText(const QString& t);
	void setDateTime(const QDateTime& t);

	QString text() const;
	QDateTime dateTime() const;
	bool isMine() const;

	QList<ChatMessageAttachment> attachments() const;
	void setAttachments(const QList<ChatMessageAttachment> &newAttachments);

signals:
	void textChanged();
	void dateChanged();
	void attachmentsChanged();

private slots:
	void onTextChanged();
	void onDateChanged();
	void onAttachmentsChanged();
	void onMenuCalled(const QPoint& pos);
	void onReply();
	void onForward();
	void onCopyText();
	void onDelete();

protected:
	void paintEvent(QPaintEvent *event) override;
	void onThemeChanged(const ThemeData& theme) override;

private:
	DropdownMenu m_Menu;

	QString m_Text;
	QDateTime m_DateTime;
	bool m_Mine;
	QList<ChatMessageAttachment> m_Attachments;
	QList<AttachmentButtonWidget*> m_AttachmentButtons;

private:
	void onDownloadClicked(uint64 id);

	int maxBubbleWidth();
	void rebuildAttachmentButtons();
	void rebuildLayout();
};

#endif // CHATMESSAGEWIDGET_H
