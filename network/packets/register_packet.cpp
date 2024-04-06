#include "register_packet.h"
#include "../packets.h"
#include "../stream_parser.h"
#include "../stream_builder.h"


RegisterPacket::RegisterPacket()
    : IPacket(REGISTER_PACKET)
{

}

RegisterPacket::~RegisterPacket() { }

void RegisterPacket::prepareToRead(const QByteArray &data)
{
    StreamParser sp(data);
    errorCode = sp.parseUInt16();
    uid = sp.parseUInt64();
    success = errorCode == 0;
}

QByteArray RegisterPacket::prepareToSend() const
{
    StreamBuilder sb;
    sb.addUInt16(id);
    sb.addUInt16(login.length());
    sb.addUInt16(password.length());
    sb.addString(login);
    sb.addString(password);
    return sb.build();
}