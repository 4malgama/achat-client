#include "account.h"
#include "../client.h"
#include "../widgets/server_message_widget.h"
#include "packets/all.h"
#include "resource_manager/resource_manager.h"
#include "../utils/json_utils.h"
#include "settings/settings_manager.h"
#include "../types/classes.h"
#include "../secure/certification/certification_manager.h"
#include "../secure/encryption/aes.h"

#include <QApplication>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QDebug>


static QVector<QString> vProtocols = {
	"1.0"
};

static bool CheckAvailabilityProtocol(const QString& protocol)
{
	return vProtocols.contains(protocol);
}

namespace client
{
	extern Client* window;
}

namespace crypto
{
	QString MD5(const QString& password);
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
	if (ServerHelloPacket* P = dynamic_cast<ServerHelloPacket*>(packet))
	{
		bool ok = false;
		if (CheckAvailabilityProtocol(P->protocol))
		{
			if (CertificationManager::verifyCertificate(P->certificate.toUtf8()))
			{
				QByteArray key = QByteArray::fromBase64(P->key.toUtf8());
				QByteArray iv = QByteArray::fromBase64(P->iv.toUtf8());
				if (key.size() == 32 && iv.size() == 16)
				{
					this->aes = new AES(AES::CBC_256);
					this->aes->setKey(key);
					this->aes->setIV(iv);
					ok = true;
					encryption = true;
				}
			}
		}
		if (!ok) handShakeFailed();
		else handShakeSuccessful();
		return;
	}
	else if (dynamic_cast<ServerReadyPacket*>(packet))
	{
		authorization();
	}
	else if (AuthRejectPacket* P = dynamic_cast<AuthRejectPacket*>(packet))
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
	else if (InitChatsPacket* P = dynamic_cast<InitChatsPacket*>(packet))
	{
		if (P->jsonData.isEmpty())
			return;
		QJsonDocument doc = QJsonDocument::fromJson(P->jsonData.toUtf8());
		QJsonObject json = doc.object();
		QJsonArray chats = json.value("chats").toArray();

		QList<InitChatData> data;

		for (QJsonValue&& chat : chats)
		{
			QJsonObject o = chat.toObject();

			InitChatData chatData;

			chatData.id = o.value("id").toVariant().toULongLong();
			chatData.isGroup = o.value("is_group").toBool();
			if (chatData.isGroup == false)
			{
				QJsonObject u = o.value("user").toObject();
				chatData.user.uid = u.value("id").toVariant().toULongLong();
				chatData.user.fname = u.value("name").toString();
				chatData.user.sname = u.value("surname").toString();
				chatData.user.mname = u.value("patronymic").toString();
				chatData.user.post = u.value("post").toString();
				if (u.value("avatar_data").isNull() == false)
					chatData.user.avatar = QImage::fromData(QByteArray::fromBase64(u.value("avatar_data").toString().toUtf8()));
			}

			data.append(chatData);
		}

		client::window->initChats(data);

		return;
	}
	else if (InitMessagesPacket* P = dynamic_cast<InitMessagesPacket*>(packet))
	{
		if (P->jsonData.isEmpty())
			return;

		QJsonDocument doc = QJsonDocument::fromJson(P->jsonData.toUtf8());
		QJsonObject json = doc.object();
		QJsonArray messages = json.value("messages").toArray();
		quint64 chatId = json.value("chat_id").toVariant().toULongLong();

		QList<ChatMessage> data;

		for (QJsonValue&& message : messages)
		{
			QJsonObject o = message.toObject();
			ChatMessage msg;

			msg.id = o.value("id").toVariant().toULongLong();
			msg.timestamp = o.value("time").toVariant().toULongLong();
			msg.content = o.value("content").toString();
			msg.replyId = o.value("reply_id").toVariant().toULongLong();
			msg.forwardId = o.value("forward_id").toVariant().toULongLong();

			QJsonObject u = o.value("sender").toObject();

			msg.user.uid = u.value("id").toVariant().toULongLong();
			msg.user.fname = u.value("name").toString();
			msg.user.sname = u.value("surname").toString();
			msg.user.mname = u.value("patronymic").toString();

			data.append(msg);
		}

		std::for_each(data.begin(), data.end(), [&data] (ChatMessage& msg) {
			if (msg.replyId)
			{
				ChatMessage& reply = *std::find_if(data.begin(), data.end(), [&msg] (ChatMessage& m) -> bool {
					return m.id == msg.replyId;
				});
				msg.replyMsg = &reply;
			}
			if (msg.forwardId)
			{
				ChatMessage& fwd = *std::find_if(data.begin(), data.end(), [&msg] (ChatMessage& m) -> bool {
					return m.id == msg.forwardId;
				});
				msg.forwardMsg = &fwd;
			}
		});

		client::window->initMessages(chatId, data);

		return;
	}
	else if (NewMessagePacket* P = dynamic_cast<NewMessagePacket*>(packet))
	{
		if (P->jsonData.isEmpty())
			return;

		QJsonDocument doc = QJsonDocument::fromJson(P->jsonData.toUtf8());
		QJsonObject json = doc.object();

		quint64 chatId = json.value("chat_id").toVariant().toULongLong();

		ChatMessage msg;
		msg.id = json.value("id").toVariant().toULongLong();
		msg.timestamp = json.value("time").toVariant().toULongLong();
		msg.content = json.value("content").toString();
		msg.replyId = json.value("reply_id").toVariant().toULongLong();
		msg.forwardId = json.value("forward_id").toVariant().toULongLong();

		QJsonObject sender = json.value("sender").toObject();

		msg.user.uid = sender.value("id").toVariant().toULongLong();
		msg.user.fname = sender.value("name").toString();
		msg.user.sname = sender.value("surname").toString();
		msg.user.mname = sender.value("patronymic").toString();

		client::window->addMessageToChat(chatId, &msg);

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

void Account::requestMessages(quint64 chatId)
{
	GetInitMessagesPacket packet;
	packet.chatId = chatId;
	send(&packet);
}

void Account::sendMessage(quint64 chatId, const QString &jsonData)
{
	SendMessagePacket packet;
	packet.chatId = chatId;
	packet.jsonData = jsonData;
	send(&packet);
}

const ProfileData *Account::getData() const
{
	return &data;
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

void Account::handShake()
{
	ClientHelloPacket packet;
	send(&packet);
}

void Account::handShakeFailed()
{
	tryDisconnect();
	client::window->showMessage(tr("An error occurred while trying to\nestablish a secure connection."), 2);
}

void Account::handShakeSuccessful()
{
	ClientReadyPacket packet;
	sendOpen(&packet);
}

void Account::authorization()
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

void Account::disconnectEvent()
{
	ServerMessageWidget::open(client::window, "No connection.");
	client::window->closeAuthWindow();
	client::window->disableSideButtons();
}

void Account::connectedEvent()
{
	handShake();
}

void Account::failConnect()
{
	ServerMessageWidget::open(client::window, "Connect timeout.\nCheck your internet connection.\nTry again.");
	client::window->disableSideButtons();
}
