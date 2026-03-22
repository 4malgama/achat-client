#ifndef VOIPSERVICE_H
#define VOIPSERVICE_H

#include <QObject>
#include <QUdpSocket>
#include <QAudioInput>
#include <QAudioOutput>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QHostAddress>
#include <QIODevice>
#include <QTimer>
#include <QByteArray>
#include <memory>


class VoIPService : public QObject
{
	Q_OBJECT

public:
	static const quint16 AutoPort;

	explicit VoIPService(QObject *parent = nullptr);
	~VoIPService() override = default;

	void startAudio();
	void stopAudio();

	quint16 getLocalPort() const;
	void setLocalPort(quint16 newLocalPort);

	quint16 getTargetPort() const;
	void setTargetPort(quint16 newTargetPort);

	QHostAddress getTargetAddress() const;
	void setTargetAddress(const QHostAddress &newTargetAddress);
	void setTargetAddress(const QString &newTargetAddress);

	bool isStarted() const;

private:
	void startAudioInput();
	void startAudioOutput();
	bool validateSettings() const;
	void resetAudioBufferIfNeeded();

private:
	QUdpSocket* socket = nullptr;
	QHostAddress targetAddress;
	quint16 targetPort = 0;
	quint16 localPort = 0;

	bool portIsSet = false;
	bool started = false;

	std::unique_ptr<QAudioInput> audioInput;
	std::unique_ptr<QAudioOutput> audioOutput;
	QAudioFormat audioFormat;
	QAudioDeviceInfo inDevice;
	QAudioDeviceInfo outDevice;

	QIODevice* mic = nullptr;
	QIODevice* speaker = nullptr;
	QTimer* playTimer = nullptr;

	QByteArray audioBuffer;

	static constexpr int PlaybackChunkSize = 2048;
	static constexpr int PlaybackIntervalMs = 20;
	static constexpr int MaxBufferedAudio = PlaybackChunkSize * 50;
};

#endif // VOIPSERVICE_H
