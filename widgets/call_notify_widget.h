#ifndef CALLNOTIFYWIDGET_H
#define CALLNOTIFYWIDGET_H

#include <QWidget>

class QToolButton;

class CallNotifyWidget : public QWidget
{
	Q_OBJECT
public:
	enum CallState
	{
		INCOMING,
		OUTGOING,
		RESET
	};

	explicit CallNotifyWidget(QWidget *parent = nullptr);

protected:
	void paintEvent(QPaintEvent*) override;
	void resizeEvent(QResizeEvent*) override;

public:
	CallState callState() const;
	void setCallState(CallState newCallState);

	QString callerName() const;
	void setCallerName(const QString &newCallerName);

	QImage callerImage() const;
	void setCallerImage(const QImage &newCallerImage);

signals:
	void callStateChanged();

	void callerNameChanged();

	void callerImageChanged();

private:
	QToolButton *acceptButton;
	QToolButton *rejectButton;

	CallState m_callState;
	QString m_callerName;
	QImage m_callerImage;
	Q_PROPERTY(CallState callState READ callState WRITE setCallState NOTIFY callStateChanged FINAL)
	Q_PROPERTY(QString callerName READ callerName WRITE setCallerName NOTIFY callerNameChanged FINAL)
	Q_PROPERTY(QImage callerImage READ callerImage WRITE setCallerImage NOTIFY callerImageChanged FINAL)
};

#endif // CALLNOTIFYWIDGET_H
