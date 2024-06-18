#ifndef SEARCHPACKET_H
#define SEARCHPACKET_H

#include "ipacket.h"
#include <QString>

class SearchPacket : public IPacket
{
public:
	QString json;

	SearchPacket();
	virtual ~SearchPacket();

	void prepareToRead(const QByteArray& data) override;
	QByteArray prepareToSend() const override;
};

#endif // SEARCHPACKET_H
