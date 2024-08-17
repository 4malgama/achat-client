#include "update_chat_id_packet.h"
#include "../packets.h"
#include "../stream_parser.h"


UpdateChatIdPacket::UpdateChatIdPacket()
	: IPacket(UPDATE_CHAT_ID_PACKET)
{
}

UpdateChatIdPacket::~UpdateChatIdPacket()
{
}

QByteArray UpdateChatIdPacket::prepareToSend() const
{
	PACKET_NO_WRITE
}

void UpdateChatIdPacket::prepareToRead(const QByteArray &data)
{
	StreamParser parser(data);
	chatId = parser.parseUInt64();
	int length = parser.parseUInt16();
	login = parser.parseString(length);
}
