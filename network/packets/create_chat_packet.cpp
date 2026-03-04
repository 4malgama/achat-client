#include "create_chat_packet.h"
#include "../packets.h"
#include "../stream_parser.h"


CreateChatPacket::CreateChatPacket()
	: IPacket(CREATE_CHAT_PACKET)
{

}

CreateChatPacket::~CreateChatPacket()
{
}

QByteArray CreateChatPacket::prepareToSend() const
{
	PACKET_NO_WRITE
}

void CreateChatPacket::prepareToRead(const QByteArray &data)
{
	StreamParser parser(data);
	int length = parser.parseUInt32();
	if (length == 0)
		return;
	jsonData = parser.parseString(length);
}
