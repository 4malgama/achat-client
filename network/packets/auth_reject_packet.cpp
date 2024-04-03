#include "auth_reject_packet.h"
#include "../packets.h"
#include "../stream_parser.h"

AuthRejectPacket::AuthRejectPacket()
    : IPacket(AUTH_REJECT_PACKET)
{

}

AuthRejectPacket::~AuthRejectPacket()
{

}

QByteArray AuthRejectPacket::prepareToSend() const
{
    return QByteArray(); //client not send packet
}

void AuthRejectPacket::prepareToRead(const QByteArray &data)
{
    StreamParser parser(data);
    quint16 length = parser.parseUInt16();
    reason = parser.parseString(length);
}