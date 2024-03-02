#include "packet_factory.h"
#include "packets/auth_packet.h"
#include "packets.h"


IPacket *PacketFactory::makePacket(quint32 id)
{
	switch (id)
	{
		case AUTH_PACKET:
			return new AuthPacket;
			break;
		default:
			return nullptr;
			break;
	}
}
