#include "voip_service.h"
#include <QTimer>


const quint16 VoIPService::AutoPort = 0;

void VoIPService::startAudioInput()
{
	inDevice = QAudioDeviceInfo::defaultInputDevice();

	audioInput = std::make_unique<QAudioInput>(inDevice, audioFormat);
	mic = audioInput->start();

	connect(mic, &QIODevice::readyRead, this, [=] () {
		QByteArray data = mic->readAll();
		socket->writeDatagram(data, targetAddress, targetPort);
	});
}

void VoIPService::startAudioOutput()
{
	connect(socket, &QUdpSocket::readyRead, this, [=] () {
		for ( ; socket->hasPendingDatagrams() ; )
		{
			QByteArray datagram;
			datagram.resize(socket->pendingDatagramSize());
			socket->readDatagram(datagram.data(), datagram.size());
			audioBuffer.append(datagram);
		}
	});

	outDevice = QAudioDeviceInfo::defaultOutputDevice();
	audioOutput = std::make_unique<QAudioOutput>(outDevice, audioFormat);
	speaker = audioOutput->start();

	QTimer* playTimer = new QTimer(this);
	connect(playTimer, &QTimer::timeout, this, [=] () {
		if (audioBuffer.size() > 2048)
		{
			speaker->write(audioBuffer.left(2048));
			audioBuffer.remove(0, 2048);
		}
	});
	playTimer->start(20);
}

VoIPService::VoIPService(QObject *parent)
	: QObject{parent}
{
	socket = new QUdpSocket(this);

	audioFormat.setSampleRate(48000);
	audioFormat.setChannelCount(1);
	audioFormat.setSampleSize(16);
	audioFormat.setSampleType(QAudioFormat::SignedInt);
	audioFormat.setByteOrder(QAudioFormat::LittleEndian);
	audioFormat.setCodec("audio/pcm");
}

void VoIPService::startAudio()
{
	if (portIsset == false)
	{
		qWarning() << "VoIP Error: Port isn\'t set. Use setLocalPort().";
		return;
	}

	startAudioInput();
	startAudioOutput();
}

quint16 VoIPService::getLocalPort() const
{
	return localPort;
}

void VoIPService::setLocalPort(quint16 newLocalPort)
{
	if (socket->state() != QAbstractSocket::UnconnectedState)
		socket->close();

	bool ok = socket->bind(QHostAddress::Any,
						   newLocalPort,
						   QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);

	if (!ok) qWarning() << "Bind failed:" << socket->errorString();

	localPort = socket->localPort();
}

quint16 VoIPService::getTargetPort() const
{
	return targetPort;
}

void VoIPService::setTargetPort(quint16 newTargetPort)
{
	targetPort = newTargetPort;
}

QHostAddress VoIPService::getTargetAddress() const
{
	return targetAddress;
}

void VoIPService::setTargetAddress(const QHostAddress &newTargetAddress)
{
	targetAddress = newTargetAddress;
}

void VoIPService::setTargetAddress(const QString &newTargetAddress)
{
	targetAddress = QHostAddress(newTargetAddress);
}
