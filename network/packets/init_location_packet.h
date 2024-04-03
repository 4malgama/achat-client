#ifndef INITLOCATIONPACKET_H
#define INITLOCATIONPACKET_H

#include "ipacket.h"
#include <QString>

class InitLocationPacket : public IPacket
{
public:
	QString location;

	InitLocationPacket();
	~InitLocationPacket() override;

	QByteArray prepareToSend() const override;
	void prepareToRead(const QByteArray& data) override;
};

#endif // INITLOCATIONPACKET_H
