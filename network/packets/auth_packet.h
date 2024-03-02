#ifndef AUTHPACKET_H
#define AUTHPACKET_H

#include "ipacket.h"
#include <QString>

class AuthPacket : public IPacket
{
public:
	QString login;

	AuthPacket();
	~AuthPacket() override;

	QByteArray prepareToSend() const override;
	void prepareToRead(const QByteArray& data) const override;
};

#endif // AUTHPACKET_H
