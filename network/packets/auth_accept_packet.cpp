#include "auth_accept_packet.h"
#include "../packets.h"

AuthAcceptPacket::AuthAcceptPacket()
    : IPacket(AUTH_ACCEPT_PACKET)
{

}

AuthAcceptPacket::~AuthAcceptPacket()
{

}

QByteArray AuthAcceptPacket::prepareToSend() const
{
    return QByteArray();
}

void AuthAcceptPacket::prepareToRead(const QByteArray &data)
{
    Q_UNUSED(data)
}