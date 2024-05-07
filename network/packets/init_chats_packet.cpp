#include "init_chats_packet.h"
#include "../packets.h"
#include "../stream_parser.h"


InitChatsPacket::InitChatsPacket()
	: IPacket(INIT_CHATS_PACKET)
{

}

InitChatsPacket::~InitChatsPacket()
{

}

QByteArray InitChatsPacket::prepareToSend() const
{
	return QByteArray();
}

void InitChatsPacket::prepareToRead(const QByteArray &data)
{
	StreamParser parser(data);

	quint32 length = parser.parseUInt32();
	jsonData = parser.parseString(length);
}
