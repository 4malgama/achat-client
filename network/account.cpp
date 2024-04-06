#include "account.h"
#include "../client.h"
#include "../widgets/server_message_widget.h"
#include "packets/all.h"
#include "resource_manager/resource_manager.h"
#include "../utils/json_utils.h"
#include "settings/settings_manager.h"

#include <QApplication>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>

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

namespace auth
{
	extern bool remember;
	extern QString login;
	extern QString password;
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
		client::window->authWindow();
		return;
	}
	else if (AuthAcceptPacket* P = dynamic_cast<AuthAcceptPacket*>(packet))
	{
		data.uid = P->uid;
		onLoginSuccess();
		return;
	}
	else if (RegisterPacket* P = dynamic_cast<RegisterPacket*>(packet))
	{
		if (P->success)
		{
			data.uid = P->uid;
			onLoginSuccess();
		}
		else
		{
			QString reason = P->errorCode == 1 ? tr("Login already exists") : tr("Unsafe password.");

			client::window->showMessage(tr("Registration failed.\nReason: %1.").arg(reason), 2);
			client::window->disableSideButtons();
			client::window->authWindow();
		}
		return;
	}
	else if (UpdateAvatarPacket* P = dynamic_cast<UpdateAvatarPacket*>(packet))
	{
		if (P->avatarData.isEmpty() == false)
		{
			ResourceManager& rm = ResourceManager::instance();
			rm.setAvatarData(P->avatarData);
		}
		return;
	}
	else if (InitProfilePacket* P = dynamic_cast<InitProfilePacket*>(packet))
	{
		if (P->profileData.isEmpty() == false)
		{
			client::window->setProfileData(P->profileData);
		}
		return;
	}
}

void Account::updateProfile(const QHash<QString, QVariant>& profileInfo)
{
	if (profileInfo.isEmpty() == false)
	{
		UpdateProfilePacket packet;
		packet.changes = JsonUtils::jsonToString(JsonUtils::hashmapToJson(profileInfo));
		send(&packet);
	}
}

void Account::onLoginSuccess()
{
	client::window->closeAuthWindow();
	client::window->enableSideButtons();

	if (auth::remember)
	{
		SettingsManager::getInstance().setLastUid(data.uid);
	}
	else
	{
		SettingsManager::getInstance().setLastUid(0);
	}

	ResourceManager& rm = ResourceManager::instance();
	rm.initUser(data.uid, auth::remember, auth::login, auth::password);

	data.avatar = rm.getAvatar();

	CheckAvatarHashPacket packet;
	packet.hash = crypto::FileHash(rm.getAvatarPath());
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
	QLocale locale = QLocale::system();
	QString localeName = locale.name();

	InitLocationPacket packet;
	packet.location = localeName.mid(localeName.indexOf('_') + 1);
	send(&packet);

	quint64 uid = SettingsManager::getInstance().getLastUid();
	if (uid != 0)
	{
		ResourceManager::instance().initUser(uid);
		QPair<QString, QString> pair = ResourceManager::instance().getAutoLoginData();
		if (pair.first.isEmpty() || pair.second.isEmpty())
		{
			client::window->authWindow();
			return;
		}
		auth::remember = true;
		auth::login = pair.first;
		auth::password = pair.second;
		login(pair.first, pair.second);
		return;
	}

	client::window->authWindow();
}

void Account::failConnect()
{
	ServerMessageWidget::open(client::window, "Connect timeout.\nCheck your internet connection.\nTry again.");
	client::window->disableSideButtons();
}
