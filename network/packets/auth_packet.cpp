#include "auth_packet.h"
#include "../packets.h"
#include "../stream_builder.h"

#include <QDebug>


AuthPacket::AuthPacket()
	: IPacket(AUTH_PACKET)
{

}

AuthPacket::~AuthPacket()
{

}

QByteArray AuthPacket::prepareToSend() const
{
	StreamBuilder sb;
	sb.addUInt16(id);
	sb.addBool(remember);
	sb.addUInt16(login.length());
	sb.addUInt16(password.length());
	sb.addString(login);
	sb.addString(password);
	return sb.build();
}

void AuthPacket::prepareToRead(const QByteArray &data)
{
	Q_UNUSED(data)
}
