#ifndef INITPROFILEPACKET_H
#define INITPROFILEPACKET_H

#include "ipacket.h"
#include <QHash>
#include <QVariant>


class InitProfilePacket : public IPacket
{
public:
	QHash<QString, QVariant> profileData;

	InitProfilePacket();
	~InitProfilePacket() override;

	QByteArray prepareToSend() const override;
	void prepareToRead(const QByteArray& data) override;
};

#endif // INITPROFILEPACKET_H
