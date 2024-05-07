#ifndef CHATMESSAGEWIDGET_H
#define CHATMESSAGEWIDGET_H

#include <QWidget>
#include <QDateTime>

class ChatMessageWidget : public QWidget
{
	Q_OBJECT
public:
	explicit ChatMessageWidget(QWidget *parent = nullptr, bool isMine = true);

	void setText(const QString& t);
	void setDateTime(const QDateTime& t);

	QString text() const;
	QDateTime dateTime() const;
	bool isMine() const;

signals:
	void textChanged();
	void dateChanged();

private slots:
	void onTextChanged();
	void onDateChanged();

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

	void paintEvent(QPaintEvent *event);
};

#endif // CHATMESSAGEWIDGET_H
