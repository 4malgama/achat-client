#include "incoming_call_packet.h"
#include "../packets.h"
#include "../stream_builder.h"
#include "../stream_parser.h"


IncomingCallPacket::IncomingCallPacket()
	: IPacket(INCOMING_CALL_PACKET)
{

}

IncomingCallPacket::~IncomingCallPacket()
{

}

QByteArray IncomingCallPacket::prepareToSend() const
{
	StreamBuilder sb;
	sb.addUInt16(id);
	sb.addUInt64(targetId);
	sb.addUInt16(port);
	return sb.build();
}

void IncomingCallPacket::prepareToRead(const QByteArray &data)
{
	StreamParser parser(data);
	targetId = parser.parseUInt64();
	port = parser.parseUInt16();
}
