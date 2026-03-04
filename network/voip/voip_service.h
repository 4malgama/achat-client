#ifndef VOIPSERVICE_H
#define VOIPSERVICE_H

#include <QObject>
#include <QUdpSocket>
#include <QAudioInput>
#include <QAudioOutput>
#include <QAudioFormat>
#include <QAudioDeviceInfo>

#include <memory>


class VoIPService : public QObject
{
	Q_OBJECT

	void startAudioInput();
	void startAudioOutput();

public:
	static const quint16 AutoPort;

	explicit VoIPService(QObject *parent = nullptr);
	void startAudio();

	quint16 getLocalPort() const;
	void setLocalPort(quint16 newLocalPort);

	quint16 getTargetPort() const;
	void setTargetPort(quint16 newTargetPort);

	QHostAddress getTargetAddress() const;
	void setTargetAddress(const QHostAddress &newTargetAddress);
	void setTargetAddress(const QString& newTargetAddress);

private:
	QUdpSocket* socket;
	QHostAddress targetAddress;
	quint16 targetPort;
	quint16 localPort;

	bool portIsset = false;

	std::unique_ptr<QAudioInput> audioInput;
	std::unique_ptr<QAudioOutput> audioOutput;
	QAudioFormat audioFormat;
	QAudioDeviceInfo inDevice;
	QAudioDeviceInfo outDevice;

	QIODevice* mic;
	QIODevice* speaker;

	QByteArray audioBuffer;
};

#endif // VOIPSERVICE_H
