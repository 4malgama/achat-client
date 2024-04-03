#ifndef CHECKAVATARHASHPACKET_H
#define CHECKAVATARHASHPACKET_H

#include "ipacket.h"
#include <QString>

class CheckAvatarHashPacket : public IPacket
{
public:
	QString hash;

	CheckAvatarHashPacket();
	~CheckAvatarHashPacket() override;

	QByteArray prepareToSend() const override;
	void prepareToRead(const QByteArray& data) override;
};

#endif // CHECKAVATARHASHPACKET_H
