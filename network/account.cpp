#include "account.h"
#include "../client.h"
#include "packets/auth_packet.h"
#include "../widgets/server_message_widget.h"
#include <QMessageBox>


namespace client
{
	extern Client* window;
}

Account::Account(QObject *parent)
	: Network{parent}
{

}

Account::~Account()
{

}

void Account::start()
{
	tryConnect();
}

void Account::login(const QString &login, const QString &password)
{
	if (isConnected() == false)
		return;

	AuthPacket packet;
	packet.login = login;

	send(&packet);
}

void Account::readEvent(const IPacket* packet)
{

}

void Account::disconnectEvent()
{
	ServerMessageWidget::exec(client::window, "No connection.");
	client::window->disableSideButtons();
}

void Account::connectedEvent()
{
	qInfo() << "Connected.";
	client::window->authWindow();
}

void Account::failConnect()
{
	//(new ServerMessageWidget(client::window, "Connect timeout.\nCheck your internet connection.\nTry again."))->exec();
	ServerMessageWidget::exec(client::window, "Connect timeout.\nCheck your internet connection.\nTry again.");
	client::window->disableSideButtons();
}
