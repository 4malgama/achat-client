#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "network.h"

class Account : public Network
{
	QString token;
	//TODO Profile Data

public:
	explicit Account(QObject *parent = nullptr);
	~Account();

	void start();
	void login(const QString& login, const QString& password);

private:
	void readEvent(const IPacket* packet) override;
	void disconnectEvent() override;
	void connectedEvent() override;
	void failConnect() override;
};

#endif // ACCOUNT_H
