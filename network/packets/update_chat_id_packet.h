#ifndef UPDATECHATIDPACKET_H
#define UPDATECHATIDPACKET_H

#include "ipacket.h"
#include <QString>

class UpdateChatIdPacket : public IPacket
{
public:
	quint8 chatId;
	QString login;

	UpdateChatIdPacket();
	~UpdateChatIdPacket();

	QByteArray prepareToSend() const override;
	void prepareToRead(const QByteArray& data) override;
};

#endif // UPDATECHATIDPACKET_H
