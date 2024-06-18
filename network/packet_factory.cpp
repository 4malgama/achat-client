#include "packet_factory.h"
#include "packets/all.h"
#include "packets.h"


std::unique_ptr<IPacket> PacketFactory::createPacket(uint32_t id)
{
	switch (id)
	{
		case AUTH_PACKET				: return std::make_unique<AuthPacket>();
		case AUTH_REJECT_PACKET			: return std::make_unique<AuthRejectPacket>();
		case AUTH_ACCEPT_PACKET			: return std::make_unique<AuthAcceptPacket>();
		case REGISTER_PACKET			: return std::make_unique<RegisterPacket>();
		case SEND_TOKEN_PACKET			: return std::make_unique<SendTokenPacket>();
		case INIT_LOCATION_PACKET		: return std::make_unique<InitLocationPacket>();
		case CHECK_AVATAR_HASH_PACKET	: return std::make_unique<CheckAvatarHashPacket>();
		case UPDATE_AVATAR_PACKET		: return std::make_unique<UpdateAvatarPacket>();
		case INIT_PROFILE_PACKET		: return std::make_unique<InitProfilePacket>();
		case UPDATE_PROFILE_PACKET		: return std::make_unique<UpdateProfilePacket>();
		case INIT_CHATS_PACKET			: return std::make_unique<InitChatsPacket>();
		case GET_INIT_MESSAGES_PACKET	: return std::make_unique<GetInitMessagesPacket>();
		case INIT_MESSAGES_PACKET		: return std::make_unique<InitMessagesPacket>();
		case SEND_MESSAGE_PACKET		: return std::make_unique<SendMessagePacket>();
		case NEW_MESSAGE_PACKET			: return std::make_unique<NewMessagePacket>();
		case CLIENT_HELLO_PACKET		: return std::make_unique<ClientHelloPacket>();
		case SERVER_HELLO_PACKET		: return std::make_unique<ServerHelloPacket>();
		case CLIENT_READY_PACKET		: return std::make_unique<ClientReadyPacket>();
		case SERVER_READY_PACKET		: return std::make_unique<ServerReadyPacket>();
		case SEARCH_PACKET				: return std::make_unique<SearchPacket>();
		default							: return nullptr;
	}
}
