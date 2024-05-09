#ifndef INITMESSAGESPACKET_H
#define INITMESSAGESPACKET_H

#include "ipacket.h"
#include <QString>

class InitMessagesPacket : public IPacket
{
public:
	QString jsonData;

	InitMessagesPacket();
	~InitMessagesPacket();

	QByteArray prepareToSend() const override;
	void prepareToRead(const QByteArray& data) override;
};

#endif // INITMESSAGESPACKET_H
