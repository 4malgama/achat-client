#include "new_message_packet.h"
#include "../packets.h"
#include "../stream_parser.h"


NewMessagePacket::NewMessagePacket()
	: IPacket(NEW_MESSAGE_PACKET)
{

}

NewMessagePacket::~NewMessagePacket()
{

}

QByteArray NewMessagePacket::prepareToSend() const
{
	PACKET_NO_WRITE
}

void NewMessagePacket::prepareToRead(const QByteArray &data)
{
	StreamParser sp(data);
	int length = sp.parseInt32();
	jsonData = sp.parseString(length);
}

