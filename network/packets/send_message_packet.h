#ifndef SENDMESSAGEPACKET_H
#define SENDMESSAGEPACKET_H

#include "ipacket.h"
#include <QString>

class SendMessagePacket : public IPacket
{
public:
	quint64 chatId;
	QString jsonData;

	SendMessagePacket();
	~SendMessagePacket();

	QByteArray prepareToSend() const;
	void prepareToRead(const QByteArray& data);
};

#endif // SENDMESSAGEPACKET_H
