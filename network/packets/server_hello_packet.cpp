#include "server_hello_packet.h"
#include "../packets.h"

#include <QtEndian>
#include <stdexcept>


ServerHelloPacket::ServerHelloPacket()
	: IPacket(SERVER_HELLO_PACKET)
{

}

ServerHelloPacket::~ServerHelloPacket()
{

}

QByteArray ServerHelloPacket::prepareToSend() const
{
	PACKET_NO_WRITE
}

void ServerHelloPacket::prepareToRead(const QByteArray &data)
{
	protocol.clear();

	if (data.size() < 2)
		throw std::runtime_error("Truncated ServerHello");

	const quint16 length = qFromBigEndian<quint16>(data.constData());

	if (length == 0 || length > 32)
		throw std::runtime_error("Invalid protocol version length");

	const qsizetype expectedSize = 2 + static_cast<qsizetype>(length) * 2;

	if (data.size() != expectedSize)
		throw std::runtime_error("Invalid ServerHello size");

	protocol.reserve(length);

	for (quint16 i = 0; i < length; ++i)
	{
		const quint16 codeUnit = qFromBigEndian<quint16>(
			data.constData() + 2 + static_cast<qsizetype>(i) * 2
		);

		protocol.append(QChar(codeUnit));
	}
}
