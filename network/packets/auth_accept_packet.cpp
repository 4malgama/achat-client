#include "auth_accept_packet.h"
#include "../packets.h"
#include "../stream_parser.h"

#include <QDebug>

AuthAcceptPacket::AuthAcceptPacket()
	: IPacket(AUTH_ACCEPT_PACKET)
{

}

AuthAcceptPacket::~AuthAcceptPacket()
{

}

QByteArray AuthAcceptPacket::prepareToSend() const
{
	return QByteArray();
}

void AuthAcceptPacket::prepareToRead(const QByteArray &data)
{
	StreamParser sp(data);
	uid = sp.parseUInt64();
	bool hasToken = sp.parseByte();
	if (hasToken)
	{
		quint16 length = sp.parseUInt16();
		token = sp.parseString(length);
	}
}
