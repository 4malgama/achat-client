#include "init_messages_packet.h"
#include "../packets.h"
#include "../stream_parser.h"


InitMessagesPacket::InitMessagesPacket()
	: IPacket(INIT_MESSAGES_PACKET)
{

}

InitMessagesPacket::~InitMessagesPacket()
{

}

QByteArray InitMessagesPacket::prepareToSend() const
{
	return QByteArray();
}

void InitMessagesPacket::prepareToRead(const QByteArray &data)
{
	StreamParser sp(data);
	quint64 length = sp.parseUInt32();
	jsonData = sp.parseString(length);
}
