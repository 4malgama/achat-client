#ifndef SERVER_MESSAGE_WIDGET_H
#define SERVER_MESSAGE_WIDGET_H

#include <QDialog>

namespace Ui {
	class ServerMessageWidget;
}

class ServerMessageWidget : public QDialog
{
	Q_OBJECT

	QString text;

public:
	explicit ServerMessageWidget(QWidget *parent, const QString& text);
	~ServerMessageWidget();

	int exec();

	static int exec(QWidget* parent, const QString& text);

private:
	Ui::ServerMessageWidget *ui;
	void paintEvent(QPaintEvent *event);
};

#endif // SERVER_MESSAGE_WIDGET_H
