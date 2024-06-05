#ifndef AUTHACCEPTPACKET_H
#define AUTHACCEPTPACKET_H

#include "ipacket.h"
#include <QString>

class AuthAcceptPacket : public IPacket
{
public:
	quint64 uid;
	QString token;

	AuthAcceptPacket();
	~AuthAcceptPacket() override;

	QByteArray prepareToSend() const override;
	void prepareToRead(const QByteArray& data) override;
};

#endif // AUTHACCEPTPACKET_H
