#include "network.h"
#include "packet_factory.h"
#include "packets/ipacket.h"

#include <QTimer>
#include <QSslConfiguration>
#include <QSslError>
#include <QScopedValueRollback>
#include <QtEndian>

#include <exception>

#include <QDebug>


namespace console
{
	void writeLine(const QString& text);
	extern bool isViewPackets;
}


Network::Network(QObject *parent)
	: QObject{parent}
	, timeoutTimer(new QTimer(this))
{
	timeoutTimer->setInterval(CONNECT_TIMEOUT_MS);
	timeoutTimer->setSingleShot(true);
	timeoutTimer->setTimerType(Qt::PreciseTimer);

	socket.setReadBufferSize(1024 * 1024);	//1Mb

	connect(&socket, &QSslSocket::encrypted, this, &Network::onEncrypted);
	connect(&socket, &QAbstractSocket::stateChanged, this, &Network::onStateChanged);
	connect(&socket, &QIODevice::readyRead, this, &Network::onReadEvent);
	connect(&socket, &QAbstractSocket::errorOccurred, this, &Network::handleError);
	connect(&socket, QOverload<const QList<QSslError> &>::of(&QSslSocket::sslErrors), this, [this] (const QList<QSslError>& errors)
	{
		if (!connected && !connecting)
			return;

		QStringList descriptions;

		for (const QSslError& error : errors)
			descriptions.append(error.errorString());

		endConnection(tr("TLS certificate verification failed:\n%1").arg(descriptions.join('\n')));
	});

	connect(timeoutTimer, &QTimer::timeout, this, &Network::onTimeout);
}

Network::~Network()
{
	QObject::disconnect(&socket, nullptr, this, nullptr);
	QObject::disconnect(timeoutTimer, nullptr, this, nullptr);

	timeoutTimer->stop();

	connected = false;
	connecting = false;

	socket.abort();
	buffer.clear();
}

void Network::setInetAddress(const InetAddress &endPoint)
{
	inetAddress = endPoint;
}

bool Network::isConnected() const
{
	return connected
		&& socket.state() == QAbstractSocket::ConnectedState
		&& socket.isEncrypted();
}

QString Network::lastNetworkError() const
{
	return lastError;
}

void Network::tryConnect()
{
	tryDisconnect();

	lastError.clear();
	connecting = true;

	if (inetAddress.ip.trimmed().isEmpty() || inetAddress.port == 0)
	{
		endConnection(tr("Invalid server address or port."));
		return;
	}

	if (!QSslSocket::supportsSsl())
	{
		endConnection(tr("TLS is unavailable."));
		return;
	}

	QSslConfiguration configuration = QSslConfiguration::defaultConfiguration();

	configuration.setProtocol(QSsl::TlsV1_2OrLater);
	configuration.setPeerVerifyMode(QSslSocket::VerifyPeer);

	socket.setSslConfiguration(configuration);

	console::writeLine(
		QStringLiteral("Connecting with TLS to %1:%2")
			.arg(inetAddress.ip)
			.arg(inetAddress.port)
	);

	timeoutTimer->start();

	socket.connectToHostEncrypted(
		inetAddress.ip.trimmed(),
		inetAddress.port
	);
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

	connected = false;
	connecting = false;

	buffer.clear();
	socket.abort();
}

void Network::onEncrypted()
{
	if (!connecting)
	{
		socket.abort();
		return;
	}

	timeoutTimer->stop();

	connecting = false;
	connected = true;
	lastError.clear();

	console::writeLine(QStringLiteral("TLS connection established."));

	connectedEvent();
}

void Network::send(const IPacket *packet)
{
	if (!packet || packet->getId() == 0 || !isConnected())
		return;

	const QByteArray data = packet->prepareToSend();

	if (data.size() < 2 || data.size() > MAX_PACKET_SIZE)
	{
		endConnection(tr("Invalid outgoing packet size."));
		return;
	}

	const qint64 pending = socket.bytesToWrite() + socket.encryptedBytesToWrite();

	if (pending > MAX_PENDING_WRITE - data.size())
	{
		endConnection(tr("Outgoing network buffer limit exceeded."));
		return;
	}

	if (console::isViewPackets)
	{
		console::writeLine(
			QStringLiteral("[SEND ID=%1 SIZE=%2]")
				.arg(packet->getId())
				.arg(data.size())
		);
	}

	if (socket.write(data) != data.size())
	{
		endConnection(tr("Failed to queue the outgoing packet."));
		return;
	}
}

std::unique_ptr<IPacket> Network::getPacketByID(quint32 id)
{
	return PacketFactory::createPacket(id);
}

void Network::onStateChanged(QAbstractSocket::SocketState state)
{
	if (state != QAbstractSocket::UnconnectedState)
		return;

	if (connected || connecting)
		endConnection(tr("The connection was closed."));
}

void Network::endConnection(const QString& reason)
{
	const bool wasConnected = connected;
	const bool wasConnecting = connecting;

	lastError = reason;
	console::writeLine(reason);

	tryDisconnect();

	if (wasConnected)
		disconnectEvent();
	else if (wasConnecting)
		failConnect();
}

void Network::onReadEvent()
{
	if (!isConnected() || reading)
		return;

	QScopedValueRollback<bool> readingGuard(reading, true);

	while (isConnected() && socket.bytesAvailable() > 0)
	{
		const qint64 remaining = MAX_PACKET_SIZE - buffer.size();

		if (remaining <= 0)
		{
			endConnection(tr("Incoming network buffer limit exceeded."));
			return;
		}

		const qint64 count = qMin<qint64>(remaining, 64 * 1024);
		const QByteArray chunk = socket.read(count);

		if (chunk.isEmpty())
			return;

		buffer.append(chunk);
		processBuffer();
	}
}

void Network::onTimeout()
{
	if (connecting)
		endConnection(tr("TCP/TLS connection timeout."));
}

void Network::handleError(QAbstractSocket::SocketError socketError)
{
	Q_UNUSED(socketError);

	if (connected || connecting)
		endConnection(socket.errorString());
}

void Network::processBuffer()
{
	// [4:size][2:ID][N:data]
	while (isConnected() && buffer.size() >= 4)
	{
		const quint32 packetSize = qFromBigEndian<quint32>(buffer.constData());

		if (packetSize < 6 || packetSize > MAX_PACKET_SIZE)
		{
			endConnection(tr("Invalid incoming packet size."));
			return;
		}

		if (buffer.size() < static_cast<qsizetype>(packetSize))
			return;

		const quint16 id = qFromBigEndian<quint16>(buffer.constData() + 4);

		const QByteArray payload = buffer.mid(
			6,
			static_cast<qsizetype>(packetSize) - 6
		);

		buffer.remove(0, static_cast<qsizetype>(packetSize));

		auto packet = PacketFactory::createPacket(id);

		if (!packet)
		{
			endConnection(tr("Unknown incoming packet ID: %1").arg(id));
			return;
		}

		try
		{
			packet->prepareToRead(payload);
		}
		catch (const std::exception&)
		{
			endConnection(tr("Malformed incoming packet."));
			return;
		}
		catch (...)
		{
			endConnection(tr("Failed to decode the incoming packet."));
			return;
		}

		if (console::isViewPackets)
		{
			console::writeLine(
				QStringLiteral("[RECV ID=%1 SIZE=%2]")
					.arg(id)
					.arg(packetSize)
			);
		}

		readEvent(packet.get());
	}
}
