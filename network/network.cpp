#include "network.h"
#include "packet_factory.h"
#include "packets/ipacket.h"
#include "stream_builder.h"
#include "../secure/encryption/aes.h"
#include <QTimer>
#include <QDataStream>

#include <QDebug>

namespace crypto
{
	QByteArray MD5(const QByteArray& data);
}

const quint32 Network::TIMEOUT = 5000;

Network::Network(QObject *parent)
	: QObject{parent}
{
	connected = false;
	encryption = false;
	aes = nullptr;
	timeoutTimer = new QTimer(this);
	timeoutTimer->setInterval(TIMEOUT);
	timeoutTimer->setTimerType(Qt::PreciseTimer);
	connect(&socket, &QAbstractSocket::stateChanged, this, &Network::onStateChanged);
	connect(&socket, &QIODevice::readyRead, this, &Network::onReadEvent);
	connect(timeoutTimer, &QTimer::timeout, this, &Network::onTimeout);
	connect(&socket, &QAbstractSocket::errorOccurred, this, &Network::handleError);
}

Network::~Network()
{
	tryDisconnect();
	if (aes != nullptr)
		delete aes;
}

void Network::setInetAddress(const InetAddress &endPoint)
{
	inetAddress = endPoint;
}

bool Network::isConnected() const
{
	return connected;
}

void Network::tryConnect()
{
	socket.connectToHost(inetAddress.ip, inetAddress.port);
	timeoutTimer->start();
}

void Network::tryConnect(const QString &ip, quint16 port)
{
	inetAddress.ip = ip;
	inetAddress.port = port;
	tryConnect();
}

void Network::tryDisconnect()
{
	timeoutTimer->stop();

	if (connected)
	{
		connected = false;
		socket.disconnectFromHost();
		socket.close();
	}
}

void Network::send(const IPacket *packet)
{
	if (packet == nullptr || packet->getId() == 0)
		return;

	QByteArray data = packet->prepareToSend();
	if (encryption == true && aes != nullptr)
	{
		try {
			QByteArray cipher = aes->encrypt(data);
			QDataStream stream(&data, QIODevice::WriteOnly);
			stream << cipher;
		}
		catch (int e)
		{
			qInfo() << "Error of AES occured (" << e << ")";
			return;
		}
	}
	sendData(data);
}

void Network::sendOpen(const IPacket *packet)
{
	if (packet == nullptr || packet->getId() == 0)
		return;
	QByteArray data = packet->prepareToSend();
	sendData(data);
}

void Network::sendData(const QByteArray &data)
{
	socket.write(data);
	socket.flush();
}

void Network::onStateChanged(QAbstractSocket::SocketState state)
{
	switch (state)
		{
			case QTcpSocket::BoundState:
				break;
			case QTcpSocket::HostLookupState:
				break;
			case QTcpSocket::ClosingState:
				break;
			case QTcpSocket::ConnectedState:
			{
				timeoutTimer->stop();
				connected = true;
				connectedEvent();
				break;
			}
			case QTcpSocket::ConnectingState:
				break;
			case QTcpSocket::ListeningState:
				break;
			case QTcpSocket::UnconnectedState:
			{
				if (connected)
				{
					connected = false;
					disconnectEvent();
				}
				break;
			}
			default:
				break;
		}
}

void Network::onReadEvent()
{
	buffer.append(socket.readAll());

	for (;buffer.size() > 3;)
	{
		int packetSize = *(int*) QByteArray(buffer.mid(0, 4)).data();
		packetSize = _byteswap_ulong(packetSize);

		if (buffer.size() < packetSize)
			return;

		QByteArray data;

		if (encryption)
		{
			data = buffer.mid(4, packetSize);
			data = aes->decrypt(data);
		}
		else
		{
			data =  buffer.mid(0, packetSize);
		}

		quint16 id = *(quint16*) QByteArray(data.mid(4, 2)).data();
		id = _byteswap_ushort(id);

		std::unique_ptr<IPacket> packet = PacketFactory::createPacket(id);

		if (packet != nullptr)
		{
			packet->prepareToRead(data.mid(6, packetSize - 6));
			readEvent(packet.get());
		}

		if (encryption)
		{
			buffer.remove(0, packetSize + 4);
		}
		else
		{
			buffer.remove(0, packetSize);
		}

		if (buffer.isEmpty()) break;
	}
}

void Network::onTimeout()
{
	tryDisconnect();
	failConnect();
}

void Network::handleError(QAbstractSocket::SocketError socketError)
{
	Q_UNUSED(socketError)
	qInfo() << socket.errorString();
}
