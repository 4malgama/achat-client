#ifndef SENDTOKENPACKET_H
#define SENDTOKENPACKET_H

#include "ipacket.h"
#include <QString>

class SendTokenPacket : public IPacket
{
public:
	QString token;

	SendTokenPacket();
	~SendTokenPacket();

	QByteArray prepareToSend() const;
	void prepareToRead(const QByteArray& data);
};

#endif // SENDTOKENPACKET_H
