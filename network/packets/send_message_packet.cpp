#include "send_message_packet.h"
#include "../packets.h"
#include "../stream_builder.h"


SendMessagePacket::SendMessagePacket()
	: IPacket(SEND_MESSAGE_PACKET)
{

}

SendMessagePacket::~SendMessagePacket()
{

}

QByteArray SendMessagePacket::prepareToSend() const
{
	StreamBuilder sb;
	sb.addUInt16(id);
	sb.addUInt64(chatId);
	sb.addUInt32(jsonData.size());
	sb.addString(jsonData);
	return sb.build();
}

void SendMessagePacket::prepareToRead(const QByteArray &data)
{
	Q_UNUSED(data)
}
