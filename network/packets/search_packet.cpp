#include "search_packet.h"
#include "../packets.h"
#include "../stream_parser.h"
#include "../stream_builder.h"


SearchPacket::SearchPacket()
	: IPacket(SEARCH_PACKET)
{

}

SearchPacket::~SearchPacket()
{
}

void SearchPacket::prepareToRead(const QByteArray &data)
{
	StreamParser sp(data);
	quint32 length = sp.parseUInt32();
	json = sp.parseString(length);
}

QByteArray SearchPacket::prepareToSend() const
{
	StreamBuilder sb;
	sb.addUInt16(id);
	sb.addUInt16(json.size());
	sb.addString(json);
	return sb.build();
}
