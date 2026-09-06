#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include "themed_widget.h"

class Client;
class QPushButton;

class MessageWidget : public ThemedWidget
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
	void onThemeChanged(const ThemeData& theme) override;

signals:

};

#endif // MESSAGEWIDGET_H
