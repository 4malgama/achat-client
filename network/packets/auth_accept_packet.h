#ifndef AUTHACCEPTPACKET_H
#define AUTHACCEPTPACKET_H

#include "ipacket.h"
#include <QString>

class AuthAcceptPacket : public IPacket
{
public:
	AuthAcceptPacket();
	~AuthAcceptPacket() override;

	QByteArray prepareToSend() const override;
	void prepareToRead(const QByteArray& data) override;
};

#endif // AUTHACCEPTPACKET_H
