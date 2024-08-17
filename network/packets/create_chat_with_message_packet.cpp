#include "create_chat_with_message_packet.h"
#include "../packets.h"
#include "../stream_builder.h"


CreateChatWithMessagePacket::CreateChatWithMessagePacket()
    : IPacket(CREATE_CHAT_MESSAGE_PACKET)
{
}

CreateChatWithMessagePacket::~CreateChatWithMessagePacket()
{
}

QByteArray CreateChatWithMessagePacket::prepareToSend() const
{
	StreamBuilder sb;
	sb.addUInt16(id);
	sb.addUInt64(userId);
	sb.addUInt32(message.size());
	sb.addString(message);
	return sb.build();
}

void CreateChatWithMessagePacket::prepareToRead(const QByteArray &data)
{
	PACKET_NO_READ
}
