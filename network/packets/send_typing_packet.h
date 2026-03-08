#ifndef SEND_TYPING_PACKET_H
#define SEND_TYPING_PACKET_H

#include "ipacket.h"

class SendTypingPacket : public IPacket
{
public:
	bool isTyping;
	quint64 typingChatId;

	SendTypingPacket();
	virtual ~SendTypingPacket() override;

	virtual QByteArray prepareToSend() const override;
	virtual void prepareToRead(const QByteArray &data) override;
};

#endif // SEND_TYPING_PACKET_H
