#ifndef UPDATEPROFILEPACKET_H
#define UPDATEPROFILEPACKET_H

#include "ipacket.h"
#include <QString>

class UpdateProfilePacket : public IPacket
{
public:
	QString changes;

	UpdateProfilePacket();
	~UpdateProfilePacket() override;

	QByteArray prepareToSend() const override;
	void prepareToRead(const QByteArray& data) override;
};

#endif // UPDATEPROFILEPACKET_H
