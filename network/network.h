#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QSslSocket>
#include <QString>
#include <QByteArray>

#include <memory>

class QTimer;
class IPacket;
class AES;

class Network : public QObject
{
	Q_OBJECT

public:
	struct InetAddress
	{
		QString ip;
		quint16 port = 0;

		//InetAddress()

		InetAddress& operator= (InetAddress&& other) noexcept
		{
			ip = std::move(other.ip);
			port = std::move(other.port);
			return *this;
		}

		InetAddress& operator= (const InetAddress& other)
		{
			ip = other.ip;
			port = other.port;
			return *this;
		}
	};

	explicit Network(QObject *parent = nullptr);
	virtual ~Network();

	void setInetAddress(const InetAddress& endPoint);
	bool isConnected() const;

protected:
	virtual void readEvent(IPacket* packet) = 0;
	virtual void disconnectEvent() = 0;
	virtual void connectedEvent() = 0;
	virtual void failConnect() = 0;

	void tryConnect();
	void tryConnect(const QString& ip, quint16 port);
	void tryDisconnect();

	void send(const IPacket* packet);

	QString lastNetworkError() const;

	std::unique_ptr<IPacket> getPacketByID(quint32 id);

private:	// Variables
	static constexpr int CONNECT_TIMEOUT_MS = 15000;
	static constexpr qint64 MAX_PACKET_SIZE = 64LL * 1024 * 1024;		//64MiB
	static constexpr qint64 MAX_PENDING_WRITE = 64LL * 1024 * 1024;		//64MiB

	bool connected = false;
	bool connecting = false;
	bool reading = false;

	QSslSocket socket;
	QTimer* timeoutTimer = nullptr;

	QByteArray buffer;
	InetAddress inetAddress;
	QString lastError;

private:	// Methods
	void onStateChanged(QAbstractSocket::SocketState state);
	void onReadEvent();
	void onTimeout();
	void handleError(QAbstractSocket::SocketError socketError);
	void processBuffer();
	void onEncrypted();
	void endConnection(const QString& reason);
};

#endif // NETWORK_H
