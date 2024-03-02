#ifndef NETWORK_H
#define NETWORK_H

#include <QTcpSocket>

class QTimer;
class IPacket;

class Network : public QObject
{
	Q_OBJECT

public:
	struct InetAddress
	{
		QString ip;
		quint16 port;

		InetAddress& operator= (InetAddress&& other) noexcept
		{
			ip = other.ip;
			port = other.port;
			return *this;
		}

		InetAddress& operator= (const InetAddress& other)
		{
			ip = other.ip;
			port = other.port;
			return *this;
		}
	};

private:

	static const quint32 TIMEOUT;

	bool connected;
	QTcpSocket socket;
	QTimer* timeoutTimer;
	QByteArray buffer;

	InetAddress inetAddress;

public:
	explicit Network(QObject *parent = nullptr);
	~Network();

	void setInetAddress(const InetAddress& endPoint);

protected:
	virtual void readEvent(const IPacket* packet) = 0;
	virtual void disconnectEvent() = 0;
	virtual void connectedEvent() = 0;
	virtual void failConnect() = 0;

	void tryConnect();
	void tryConnect(const QString& ip, quint16 port);
	void tryDisconnect();
	void send(const IPacket* packet);
	void sendData(const QByteArray& data);

private:
	void onStateChanged(QAbstractSocket::SocketState state);
	void onReadEvent();
	void onTimeout();


};

#endif // NETWORK_H