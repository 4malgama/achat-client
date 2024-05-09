#include "get_init_messages_packet.h"
#include "../packets.h"
#include "../stream_builder.h"

GetInitMessagesPacket::GetInitMessagesPacket()
	: IPacket(GET_INIT_MESSAGES_PACKET)
{

}

GetInitMessagesPacket::~GetInitMessagesPacket()
{

}

QByteArray GetInitMessagesPacket::prepareToSend() const
{
	StreamBuilder sb;
	sb.addUInt16(id);
	sb.addUInt64(chatId);
	return sb.build();
}

void GetInitMessagesPacket::prepareToRead(const QByteArray &data)
{
	Q_UNUSED(data)
}
