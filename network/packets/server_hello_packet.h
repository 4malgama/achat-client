#ifndef SERVERHELLOPACKET_H
#define SERVERHELLOPACKET_H

#include "ipacket.h"
#include <QString>

class ServerHelloPacket : public IPacket
{
public:
	QString protocol;

	ServerHelloPacket();
	~ServerHelloPacket();

	QByteArray prepareToSend() const override;
	void prepareToRead(const QByteArray& data) override;
};

#endif // SERVERHELLOPACKET_H
