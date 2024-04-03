#include "update_avatar_packet.h"
#include "../packets.h"

#include <QDebug>

UpdateAvatarPacket::UpdateAvatarPacket()
	: IPacket(UPDATE_AVATAR_PACKET)
{

}

UpdateAvatarPacket::~UpdateAvatarPacket() { }

QByteArray UpdateAvatarPacket::prepareToSend() const
{
	//nothing
}

void UpdateAvatarPacket::prepareToRead(const QByteArray& data)
{
	avatarData = data;
	image = QImage::fromData(avatarData);
}