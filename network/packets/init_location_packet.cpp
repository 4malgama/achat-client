#include "init_location_packet.h"
#include "../packets.h"
#include "../stream_builder.h"

InitLocationPacket::InitLocationPacket()
    : IPacket(INIT_LOCATION_PACKET)
{

}

InitLocationPacket::~InitLocationPacket() { }

QByteArray InitLocationPacket::prepareToSend() const
{
    StreamBuilder builder;
    builder.addUInt16(id);
    builder.addUInt16(location.length());
    builder.addString(location);
    return builder.build();
}

void InitLocationPacket::prepareToRead(const QByteArray& data)
{
    //nothing
}