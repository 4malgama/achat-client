#include "network.h"
#include "packet_factory.h"
#include "packets/ipacket.h"
#include <QTimer>
#include <QDataStream>

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
	connect(&socket, &QAbstractSocket::errorOccurred, this, &Network::handleError);
}

Network::~Network()
{
	tryDisconnect();
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

		QByteArray data = buffer.mid(0, packetSize);

		quint16 id = *(quint16*) QByteArray(data.mid(4, 2)).data();
		id = _byteswap_ushort(id);

		std::unique_ptr<IPacket> packet = PacketFactory::createPacket(id);

		if (packet != nullptr)
		{
			packet->prepareToRead(data.mid(6, packetSize - 6));
			readEvent(packet.get());
		}

		buffer.remove(0, packetSize);
		if (buffer.isEmpty()) break;
	}
}

// void Network::onReadEvent()
// {
// 	buffer.append(socket.readAll());
// 	processBuffer();
// }

// void Network::processBuffer()
// {
// 	while (buffer.size() >= sizeof(quint32)) {
// 		QDataStream stream(&buffer, QIODevice::ReadOnly);
// 		quint32 packetSize;
// 		quint16 id;
// 		stream >> packetSize;
// 		stream >> id;

// 		// Check for invalid packet size
// 		if (packetSize <= 0 || buffer.size() < packetSize)
// 			return;

// 		std::unique_ptr<IPacket> packet = PacketFactory::createPacket(id);

// 		if (packet != nullptr) {
// 			QByteArray packetData = buffer.mid(sizeof(quint32) + sizeof(quint16), packetSize);
// 			packet->prepareToRead(packetData);
// 			readEvent(packet.get());
// 		}

// 		buffer.remove(0, sizeof(quint32) + sizeof(quint16) + packetSize);
// 	}
// }


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
