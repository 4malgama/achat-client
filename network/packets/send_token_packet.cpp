#include "send_token_packet.h"
#include "../packets.h"
#include "../stream_builder.h"


SendTokenPacket::SendTokenPacket()
	: IPacket(SEND_TOKEN_PACKET)
{
}

SendTokenPacket::~SendTokenPacket()
{
}

QByteArray SendTokenPacket::prepareToSend() const
{
	StreamBuilder sb;
	sb.addUInt16(id);
	sb.addUInt16(token.length());
	sb.addString(token);
	return sb.build();
}

void SendTokenPacket::prepareToRead(const QByteArray &data)
{
	PACKET_NO_READ
}
