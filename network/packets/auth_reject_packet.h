#ifndef AUTHREJECTPACKET_H
#define AUTHREJECTPACKET_H

#include "ipacket.h"
#include <QString>

class AuthRejectPacket : public IPacket
{
public:
	QString reason;

	AuthRejectPacket();
	~AuthRejectPacket() override;

	QByteArray prepareToSend() const override;
	void prepareToRead(const QByteArray& data) override;
};

#endif // AUTHREJECTPACKET_H
