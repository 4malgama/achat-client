#include "packet_factory.h"
#include "packets/all.h"
#include "packets.h"


IPacket *PacketFactory::makePacket(quint32 id)
{
	switch (id)
	{
		case AUTH_PACKET				: return new AuthPacket;
		case AUTH_REJECT_PACKET			: return new AuthRejectPacket;
		case AUTH_ACCEPT_PACKET			: return new AuthAcceptPacket;
		case REGISTER_PACKET			: return new RegisterPacket;
		case INIT_LOCATION_PACKET		: return new InitLocationPacket;
		case CHECK_AVATAR_HASH_PACKET	: return new CheckAvatarHashPacket;
		case UPDATE_AVATAR_PACKET		: return new UpdateAvatarPacket;
		case INIT_PROFILE_PACKET		: return new InitProfilePacket;
		case UPDATE_PROFILE_PACKET		: return new UpdateProfilePacket;
		default							: return nullptr;
	}
}
