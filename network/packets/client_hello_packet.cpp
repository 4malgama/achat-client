#include "client_hello_packet.h"
#include "../packets.h"
#include "../stream_builder.h"


ClientHelloPacket::ClientHelloPacket()
	: IPacket(CLIENT_HELLO_PACKET)
{

}

ClientHelloPacket::~ClientHelloPacket()
{

}

QByteArray ClientHelloPacket::prepareToSend() const
{
	StreamBuilder sb;
	sb.addUInt16(id);
	return sb.build();
}

void ClientHelloPacket::prepareToRead(const QByteArray &data)
{
	PACKET_NO_READ
}
