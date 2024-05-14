#ifndef CLIENTREADYPACKET_H
#define CLIENTREADYPACKET_H

#include "ipacket.h"

class ClientReadyPacket : public IPacket
{
public:
	ClientReadyPacket();
	~ClientReadyPacket();

	QByteArray prepareToSend() const override;
	void prepareToRead(const QByteArray& data) override;
};

#endif // CLIENTREADYPACKET_H
