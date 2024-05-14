#ifndef CLIENTHELLOPACKET_H
#define CLIENTHELLOPACKET_H

#include "ipacket.h"

class ClientHelloPacket : public IPacket
{
public:
	ClientHelloPacket();
	~ClientHelloPacket();

	QByteArray prepareToSend() const override;
	void prepareToRead(const QByteArray& data) override;
};

#endif // CLIENTHELLOPACKET_H
