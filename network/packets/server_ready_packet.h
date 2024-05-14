#ifndef SERVERREADYPACKET_H
#define SERVERREADYPACKET_H

#include "ipacket.h"

class ServerReadyPacket : public IPacket
{
public:
	ServerReadyPacket();
	~ServerReadyPacket();

	QByteArray prepareToSend() const override;
	void prepareToRead(const QByteArray& data) override;
};

#endif // SERVERREADYPACKET_H
