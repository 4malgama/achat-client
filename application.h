#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QSystemTrayIcon>

class Application : public QApplication
{
public:
	Application(int& argc, char** argv);
	void message(const QString& message);
	void message(const QString& title, const QString& text);
	void message(const QIcon& icon, const QString& title, const QString& text);

private slots:
	void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);

private:
	QSystemTrayIcon* trayIcon;
	bool ok;
};

#endif // APPLICATION_H
