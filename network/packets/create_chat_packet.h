#ifndef CREATE_CHAT_PACKET_H
#define CREATE_CHAT_PACKET_H

#include "ipacket.h"
#include <QString>


class CreateChatPacket : public IPacket
{
public:
	QString jsonData;

	CreateChatPacket();
	~CreateChatPacket();

	QByteArray prepareToSend() const override;
	void prepareToRead(const QByteArray& data) override;
};

#endif // CREATE_CHAT_PACKET_H
