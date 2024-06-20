#include "download_file_packet.h"
#include "../packets.h"
#include "../stream_builder.h"

DownloadFilePacket::DownloadFilePacket()
	: IPacket(DOWNLOAD_FILE_PACKET)
{

}

DownloadFilePacket::~DownloadFilePacket()
{

}

QByteArray DownloadFilePacket::prepareToSend() const
{
	StreamBuilder sb;
	sb.addUInt16(id);
	sb.addUInt64(fileId);
	return sb.build();
}

void DownloadFilePacket::prepareToRead(const QByteArray &data)
{
	PACKET_NO_READ
}
