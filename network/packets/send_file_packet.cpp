#include "send_file_packet.h"
#include "../packets.h"
#include "../stream_parser.h"


SendFilePacket::SendFilePacket()
	: IPacket(SEND_FILE_PACKET)
{

}

SendFilePacket::~SendFilePacket()
{

}

QByteArray SendFilePacket::prepareToSend() const
{
	PACKET_NO_WRITE
}

void SendFilePacket::prepareToRead(const QByteArray &data)
{
	StreamParser parser(data);
	quint32 length = parser.parseUInt32();
	fileData = parser.parseData(length, 1);
	quint32 length2 = parser.parseUInt32();
	fileName = parser.parseString(length2);
}
