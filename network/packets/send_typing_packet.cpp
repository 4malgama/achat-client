#include "send_typing_packet.h"
#include "../packets.h"
#include "../stream_builder.h"
#include "../stream_parser.h"


SendTypingPacket::SendTypingPacket()
	: IPacket(TYPING_PACKET)
{
}

SendTypingPacket::~SendTypingPacket()
{
}


QByteArray SendTypingPacket::prepareToSend() const
{
	StreamBuilder sb;
	sb.addUInt16(id);
	sb.addUInt64(typingChatId);
	sb.addBool(isTyping);
	return sb.build();
}

void SendTypingPacket::prepareToRead(const QByteArray &data)
{
	StreamParser sp(data);
	typingChatId = sp.parseUInt64();
	isTyping = sp.parseByte();
}
