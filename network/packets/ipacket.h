#ifndef IPACKET_H
#define IPACKET_H

#include <QByteArray>

class IPacket
{
protected:
	quint16 id;

public:
	IPacket(quint16 id);
	virtual ~IPacket();

	quint16 getId() const;

	virtual QByteArray prepareToSend() const = 0;
	virtual void prepareToRead(const QByteArray& data) const = 0;
};

#endif // IPACKET_H
