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
	void registration(const QString& login, const QString& password);
	void updateProfile(const QHash<QString, QVariant>& profileInfo);
	void requestMessages(quint64 chatId);
	void sendMessage(quint64 chatId, const QString& jsonData);

	const ProfileData* getData() const;

private:
	void readEvent(IPacket* packet) override;
	void disconnectEvent() override;
	void connectedEvent() override;
	void failConnect() override;

	void onLoginSuccess();
};

#endif // ACCOUNT_H
