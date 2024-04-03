#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include "../client.h"

class QPushButton;

class MessageWidget : public QWidget
{
	Q_OBJECT

public:
	enum MessageIcon
	{
		INFO,
		WARNING,
		ERROR,
		UNKNOWN
	} icon;

	QString message;
	QPushButton* btnClose;

	explicit MessageWidget(Client *client);

protected:
	void paintEvent(QPaintEvent *event);

signals:

};

#endif // MESSAGEWIDGET_H
