#include "server_hello_packet.h"
#include "../packets.h"
#include "../stream_parser.h"


ServerHelloPacket::ServerHelloPacket()
	: IPacket(SERVER_HELLO_PACKET)
{

}

ServerHelloPacket::~ServerHelloPacket()
{

}

QByteArray ServerHelloPacket::prepareToSend() const
{
	PACKET_NO_WRITE
}

void ServerHelloPacket::prepareToRead(const QByteArray &data)
{
	StreamParser parser(data);
	int protocolLen = parser.parseUInt16();
	int certLen = parser.parseUInt16();
	int keyLen = parser.parseUInt16();
	int ivLen = parser.parseUInt16();
	protocol = parser.parseString(protocolLen);
	certificate = parser.parseString(certLen);
	key = parser.parseString(keyLen);
	iv = parser.parseString(ivLen);
}
