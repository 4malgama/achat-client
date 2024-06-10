#include "application.h"
#include <QMenu>


namespace client
{
	void show();
}

Application::Application(int &argc, char **argv)
	: QApplication(argc, argv)
{
	if (QSystemTrayIcon::isSystemTrayAvailable() == false)
	{
		ok = false;
		return;
	}

	trayIcon = new QSystemTrayIcon(QIcon(":/r/resources/logo/achat-client_tray.png"), this);

	QMenu* trayMenu = new QMenu();
	connect(trayMenu->addAction(tr("Exit")), &QAction::triggered, this, &QApplication::quit);

	trayIcon->setContextMenu(trayMenu);
	trayIcon->setToolTip("AChat Client");

	connect(trayIcon, &QSystemTrayIcon::activated, this, &Application::onTrayIconActivated);

	trayIcon->show();
	ok = true;
}

void Application::message(const QString &message)
{
	if (!ok) return;

	trayIcon->showMessage("AChat Client", message);
}

void Application::message(const QString &title, const QString &text)
{
	if (!ok) return;

	trayIcon->showMessage(title, text);
}

void Application::message(const QIcon &icon, const QString &title, const QString &text)
{
	if (!ok) return;

	trayIcon->showMessage(title, text, icon);
}

void Application::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
	if (reason == QSystemTrayIcon::Trigger)
	{
		client::show();
	}
}
