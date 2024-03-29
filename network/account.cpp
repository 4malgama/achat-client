#include "account.h"
#include "../client.h"
#include "packets/auth_packet.h"
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
	QMessageBox::critical(client::window, "Ошибка подключения", "Нет соединения.", QMessageBox::Ok);
}

void Account::connectedEvent()
{
	qInfo() << "Connected.";
}

void Account::failConnect()
{
	int btn = QMessageBox::critical(client::window, "Не удается установить соединение с сервером", "Не удается установить соединение с сервером.\nПревышен интервал ожидания для запроса.\nПровертье подключение к интернету.", QMessageBox::Retry | QMessageBox::Close);

	if (btn == QMessageBox::Retry)
		start();
}
