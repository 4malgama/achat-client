#ifndef SENDFILEPACKET_H
#define SENDFILEPACKET_H

#include "ipacket.h"
#include <QString>

class SendFilePacket : public IPacket
{
public:
	QString fileName;
	QByteArray fileData;

	SendFilePacket();
	~SendFilePacket();

	QByteArray prepareToSend() const override;
	void prepareToRead(const QByteArray& data) override;
};

#endif // SENDFILEPACKET_H
