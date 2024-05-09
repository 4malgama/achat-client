#ifndef NEWMESSAGEPACKET_H
#define NEWMESSAGEPACKET_H

#include "ipacket.h"
#include <QString>


class NewMessagePacket : public IPacket
{
public:
	QString jsonData;

	NewMessagePacket();
	~NewMessagePacket();

	QByteArray prepareToSend() const;
	void prepareToRead(const QByteArray& data);
};

#endif // NEWMESSAGEPACKET_H
