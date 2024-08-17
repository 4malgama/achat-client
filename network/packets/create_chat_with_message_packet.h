#ifndef CREATECHATWITHMESSAGEPACKET_H
#define CREATECHATWITHMESSAGEPACKET_H

#include "ipacket.h"
#include <QString>

class CreateChatWithMessagePacket : public IPacket
{
public:
	quint64 userId;
	QString message;

	CreateChatWithMessagePacket();
	~CreateChatWithMessagePacket();

	QByteArray prepareToSend() const override;
	void prepareToRead(const QByteArray& data) override;
};

#endif // CREATECHATWITHMESSAGEPACKET_H
