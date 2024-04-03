#include "account.h"
#include "../client.h"
#include "../widgets/server_message_widget.h"
#include "packets/all.h"
#include "resource_manager/resource_manager.h"

#include <QApplication>
#include <QMessageBox>

#include <QDebug>

namespace client
{
	extern Client* window;
}

namespace crypto
{
	QString PasswordHash(const QString& password);
	QString FileHash(const QString& filePath);
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

	if (login.isEmpty() || password.isEmpty())
		return;

	AuthPacket packet;
	packet.login = login;
	packet.password = password;

	send(&packet);
}

void Account::registration(const QString &login, const QString &password)
{
	if (isConnected() == false)
		return;

	if (login.isEmpty() || password.isEmpty())
		return;

	RegisterPacket packet;
	packet.login = login;
	packet.password = password;

	send(&packet);
}

///
//  Working with packets
///

void Account::readEvent(IPacket* packet)
{
	if (AuthRejectPacket* P = dynamic_cast<AuthRejectPacket*>(packet))
	{
		client::window->showMessage(P->reason, 2);
		client::window->disableSideButtons();
		return;
	}
	else if (AuthAcceptPacket* P = dynamic_cast<AuthAcceptPacket*>(packet))
	{
		onLoginSuccess();
		return;
	}
	else if (RegisterPacket* P = dynamic_cast<RegisterPacket*>(packet))
	{
		if (P->success)
		{
			onLoginSuccess();
		}
		else
		{
			QString reason = P->errorCode == 1 ? tr("Login already exists") : tr("Unsafe password.");

			client::window->showMessage(tr("Registration failed.\nReason: %1.").arg(reason), 2);
			client::window->disableSideButtons();
		}
		return;
	}
	else if (UpdateAvatarPacket* P = dynamic_cast<UpdateAvatarPacket*>(packet))
	{
		if (P->avatarData.isEmpty() == false)
		{
			QImage avatarImage = QImage::fromData(P->avatarData);

			ResourceManager& rm = ResourceManager::instance();
			rm.setAvatar(P->image);
			rm.cacheImage("user\\avatar.jpg", P->avatarData);

			qInfo() << "cached.";
		}
		return;
	}
}

void Account::onLoginSuccess()
{
	client::window->closeAuthWindow();
	client::window->enableSideButtons();

	CheckAvatarHashPacket packet;
	packet.hash = crypto::FileHash(ResourceManager::instance().getAvatarPath());
	send(&packet);
}

void Account::disconnectEvent()
{
	ServerMessageWidget::open(client::window, "No connection.");
	//todo close auth window
	client::window->disableSideButtons();
}

void Account::connectedEvent()
{
	client::window->authWindow();
	QLocale locale = QLocale::system();
	QString localeName = locale.name();

	InitLocationPacket packet;
	packet.location = localeName.mid(localeName.indexOf('_') + 1);
	send(&packet);
}

void Account::failConnect()
{
	ServerMessageWidget::open(client::window, "Connect timeout.\nCheck your internet connection.\nTry again.");
	client::window->disableSideButtons();
}
