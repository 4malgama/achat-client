#ifndef CHATMESSAGEWIDGET_H
#define CHATMESSAGEWIDGET_H

#include <QWidget>
#include <QDateTime>
#include "../types/classes.h"

class ChatMessageWidget : public QWidget
{
	Q_OBJECT

	const int ATTACHMENT_HEIGHT;

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

private:
	struct
	{
		QColor text;
		QColor background;
		QColor date;
	} m_Colors;

	QString m_Text;
	QDateTime m_DateTime;
	bool m_Mine;
	QList<ChatMessageAttachment> m_Attachments;

	void paintEvent(QPaintEvent *event);
};

#endif // CHATMESSAGEWIDGET_H
