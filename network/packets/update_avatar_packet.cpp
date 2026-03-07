#include "update_avatar_packet.h"
#include "../packets.h"
#include "../stream_builder.h"

UpdateAvatarPacket::UpdateAvatarPacket()
	: IPacket(UPDATE_AVATAR_PACKET)
{
}

UpdateAvatarPacket::~UpdateAvatarPacket() { }

QByteArray UpdateAvatarPacket::prepareToSend() const
{
	StreamBuilder sb;
	sb.addUInt16(id);
	sb.addUInt32(avatarBase64.length());
	sb.addString(avatarBase64);
	return sb.build();
}

void UpdateAvatarPacket::prepareToRead(const QByteArray& data)
{
	avatarData = data;
	image = QImage::fromData(avatarData);
}
