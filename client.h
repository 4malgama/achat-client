#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Client; }
QT_END_NAMESPACE

class Client : public QMainWindow
{
	Q_OBJECT

public:
	Client(QWidget *parent = nullptr);
	~Client();

signals:
	void event_close();

private:
	Ui::Client *ui;
	void closeEvent(QCloseEvent*);
};
#endif // CLIENT_H
