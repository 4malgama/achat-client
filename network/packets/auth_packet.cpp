#include "auth_packet.h"
#include "../packets.h"

#include <QDebug>

AuthPacket::AuthPacket()
	: IPacket(AUTH_PACKET)
{

}

AuthPacket::~AuthPacket()
{

}

QByteArray AuthPacket::prepareToSend() const
{
	QByteArray data;

	quint16 loginLength = login.length();
	const ushort* loginData = login.utf16();

	quint16 swap_id = _byteswap_ushort(id);
	quint16 swap_length = _byteswap_ushort(loginLength);

	char16_t* swap_buffer = new char16_t[loginLength];
	for (quint16 i = 0; i < loginLength; i++)
		swap_buffer[i] = _byteswap_ushort(loginData[i]);

	QByteArray swap_login((char*)swap_buffer, loginLength * sizeof(char16_t));

	data.append((char*)&swap_id, sizeof(swap_id));
	data.append((char*)&swap_length, sizeof(swap_length));
	data.append(swap_login);

	delete[] swap_buffer;
	return data;
}

void AuthPacket::prepareToRead(const QByteArray &data) const
{
	Q_UNUSED(data)
}
