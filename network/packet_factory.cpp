#include "packet_factory.h"
#include "packets/all.h"
#include "packets.h"


IPacket *PacketFactory::makePacket(quint32 id)
{
	switch (id)
	{
		case AUTH_PACKET:
			return new AuthPacket;
			break;
		case AUTH_REJECT_PACKET:
			return new AuthRejectPacket;
			break;
		case AUTH_ACCEPT_PACKET:
			return new AuthAcceptPacket;
			break;
		case REGISTER_PACKET:
			return new RegisterPacket;
			break;
		case INIT_LOCATION_PACKET:
			return new InitLocationPacket;
			break;
		case CHECK_AVATAR_HASH_PACKET:
			return new CheckAvatarHashPacket;
			break;
		case UPDATE_AVATAR_PACKET:
			return new UpdateAvatarPacket;
			break;
		default:
			return nullptr;
			break;
	}
}
