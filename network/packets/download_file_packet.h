#ifndef DOWNLOADFILEPACKET_H
#define DOWNLOADFILEPACKET_H

#include "ipacket.h"

class DownloadFilePacket : public IPacket
{
public:
	quint64 fileId;

	DownloadFilePacket();
	~DownloadFilePacket();

	QByteArray prepareToSend() const override;
	void prepareToRead(const QByteArray& data) override;
};

#endif // DOWNLOADFILEPACKET_H
