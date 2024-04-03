#include "check_avatar_hash_packet.h"
#include "../stream_builder.h"
#include "../packets.h"

CheckAvatarHashPacket::CheckAvatarHashPacket()
    : IPacket(CHECK_AVATAR_HASH_PACKET)
{

}

CheckAvatarHashPacket::~CheckAvatarHashPacket() { }

QByteArray CheckAvatarHashPacket::prepareToSend() const
{
    StreamBuilder sb;
    sb.addUInt16(id);
    sb.addUInt16(hash.length());
    sb.addString(hash);
    return sb.build();
}


void CheckAvatarHashPacket::prepareToRead(const QByteArray& data)
{
    Q_UNUSED(data)
}