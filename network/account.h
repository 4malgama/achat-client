#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "network.h"
#include "../types/classes.h"

class Account : public Network
{
	QString token;
	ProfileData data;


public:
	explicit Account(QObject *parent = nullptr);
	virtual ~Account();

	void start();
	void login(const QString& login, const QString& password);
	void login(const QString& token);
	void registration(const QString& login, const QString& password);
	void updateProfile(const QHash<QString, QVariant>& profileInfo);
	void requestMessages(quint64 chatId);
	void sendMessage(quint64 chatId, const QString& jsonData);
	void sendSearch(const QString& searchText);
	void downloadFile(uint64 attachmentId);
	void createChat(uint64 userId);

	const ProfileData* getData() const;

private:
	void readEvent(IPacket* packet) override;
	void disconnectEvent() override;
	void connectedEvent() override;
	void failConnect() override;

	void onLoginSuccess(const QString& token = QString());

	void handShake();
	void handShakeFailed();
	void handShakeSuccessful();
	void authorization();
};

#endif // ACCOUNT_H
