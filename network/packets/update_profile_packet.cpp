#include "update_profile_packet.h"
#include "../packets.h"
#include "../stream_builder.h"

UpdateProfilePacket::UpdateProfilePacket()
    : IPacket(UPDATE_PROFILE_PACKET)
{

}


UpdateProfilePacket::~UpdateProfilePacket() { }


QByteArray UpdateProfilePacket::prepareToSend() const
{
    StreamBuilder sb;
    sb.addUInt16(id);
    sb.addUInt16(changes.length());
    sb.addString(changes);
    return sb.build();
}


void UpdateProfilePacket::prepareToRead(const QByteArray &data)
{
    Q_UNUSED(data)
}