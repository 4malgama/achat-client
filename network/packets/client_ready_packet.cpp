#include "client_ready_packet.h"
#include "../packets.h"
#include "../stream_builder.h"

ClientReadyPacket::ClientReadyPacket()
	: IPacket(CLIENT_READY_PACKET)
{

}

ClientReadyPacket::~ClientReadyPacket()
{

}

QByteArray ClientReadyPacket::prepareToSend() const
{
	StreamBuilder sb;
	sb.addUInt16(id);
	return sb.build();
}

void ClientReadyPacket::prepareToRead(const QByteArray &data)
{
	PACKET_NO_READ
}
