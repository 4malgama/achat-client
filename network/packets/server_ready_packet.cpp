#include "server_ready_packet.h"
#include "../packets.h"

ServerReadyPacket::ServerReadyPacket()
	: IPacket(SERVER_READY_PACKET)
{

}

ServerReadyPacket::~ServerReadyPacket()
{

}

QByteArray ServerReadyPacket::prepareToSend() const
{
	PACKET_NO_WRITE
}

void ServerReadyPacket::prepareToRead(const QByteArray &data)
{
	PACKET_NO_READ
}
