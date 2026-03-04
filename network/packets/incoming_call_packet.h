#ifndef INCOMINGCALLPACKET_H
#define INCOMINGCALLPACKET_H

#include "ipacket.h"
#include "../../types/types_variables.h"
#include <QString>

class IncomingCallPacket : public IPacket
{
public:
	uint16 port;
	uint64 targetId;

	IncomingCallPacket();
	~IncomingCallPacket() override;

	QByteArray prepareToSend() const override;
	void prepareToRead(const QByteArray& data) override;
};

#endif // INCOMINGCALLPACKET_H
