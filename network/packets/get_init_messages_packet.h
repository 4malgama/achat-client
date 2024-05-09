#ifndef GETINITMESSAGESPACKET_H
#define GETINITMESSAGESPACKET_H

#include "ipacket.h"

class GetInitMessagesPacket : public IPacket
{
public:
	quint64 chatId;

	GetInitMessagesPacket();
	~GetInitMessagesPacket();

	QByteArray prepareToSend() const override;
	void prepareToRead(const QByteArray& data) override;
};

#endif // GETINITMESSAGESPACKET_H
