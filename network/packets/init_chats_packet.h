#ifndef INITCHATSPACKET_H
#define INITCHATSPACKET_H

#include "ipacket.h"
#include <QString>

class InitChatsPacket : public IPacket
{
public:
	QString jsonData;

	InitChatsPacket();
	~InitChatsPacket() override;

	QByteArray prepareToSend() const override;
	void prepareToRead(const QByteArray& data) override;
};

#endif // INITCHATSPACKET_H
