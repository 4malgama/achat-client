#ifndef UPDATEAVATARPACKET_H
#define UPDATEAVATARPACKET_H

#include "ipacket.h"
#include <QByteArray>
#include <QImage>

class UpdateAvatarPacket : public IPacket
{
public:
	QByteArray avatarData;
	QImage image;

	UpdateAvatarPacket();
	~UpdateAvatarPacket() override;

	QByteArray prepareToSend() const override;
	void prepareToRead(const QByteArray& data) override;
};

#endif // UPDATEAVATARPACKET_H
