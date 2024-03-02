#include "network.h"
#include "packet_factory.h"
#include "packets/ipacket.h"
#include <QTimer>

#include <QDebug>

const quint32 Network::TIMEOUT = 5000;

Network::Network(QObject *parent)
	: QObject{parent}
{
	connected = false;
	timeoutTimer = new QTimer(this);
	timeoutTimer->setInterval(TIMEOUT);
	timeoutTimer->setTimerType(Qt::PreciseTimer);
	connect(&socket, &QAbstractSocket::stateChanged, this, &Network::onStateChanged);
	connect(&socket, &QIODevice::readyRead, this, &Network::onReadEvent);
	connect(timeoutTimer, &QTimer::timeout, this, &Network::onTimeout);
}

Network::~Network()
{
	tryDisconnect();
}

void Network::setInetAddress(const InetAddress &endPoint)
{
	inetAddress = endPoint;
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
				connectedEvent();
				connected = true;
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
					disconnectEvent();
					connected = false;
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

		QByteArray data = buffer.mid(0, packetSize);

		quint16 id = *(quint16*) QByteArray(data.mid(4, 2)).data();
		id = _byteswap_ushort(id);

		IPacket* packet = PacketFactory::makePacket(id);

		if (packet != nullptr)
		{
			packet->prepareToRead(data.mid(6, packetSize - 6));

			readEvent(packet);

			delete packet;
		}

		buffer.remove(0, packetSize);
		if (buffer.isEmpty()) break;
	}
}

void Network::onTimeout()
{
	tryDisconnect();
	failConnect();
}
