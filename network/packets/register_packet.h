#ifndef REGISTERPACKET_H
#define REGISTERPACKET_H

#include "ipacket.h"
#include <QString>

class RegisterPacket : public IPacket
{
public:
	bool success;
	quint16 errorCode;
	QString login;
	QString password;
	quint64 uid;

	RegisterPacket();
	~RegisterPacket() override;

	QByteArray prepareToSend() const override;
	void prepareToRead(const QByteArray& data) override;
};

#endif // REGISTERPACKET_H
