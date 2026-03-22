#include "voip_service.h"


namespace console
{
	void writeLine(const QString& s);
}

const quint16 VoIPService::AutoPort = 0;

VoIPService::VoIPService(QObject *parent)
	: QObject(parent)
	, socket(new QUdpSocket(this))
	, playTimer(new QTimer(this))
{
	audioFormat.setSampleRate(48000);
	audioFormat.setChannelCount(1);
	audioFormat.setSampleSize(16);
	audioFormat.setSampleType(QAudioFormat::SignedInt);
	audioFormat.setByteOrder(QAudioFormat::LittleEndian);
	audioFormat.setCodec("audio/pcm");

	playTimer->setInterval(PlaybackIntervalMs);

	connect(playTimer, &QTimer::timeout, this, [this]() {
		if (!speaker)
			return;

		if (audioBuffer.size() >= PlaybackChunkSize)
		{
			const QByteArray chunk = audioBuffer.left(PlaybackChunkSize);
			const qint64 written = speaker->write(chunk);

			if (written > 0)
				audioBuffer.remove(0, static_cast<int>(written));
		}

		resetAudioBufferIfNeeded();
	});
}

void VoIPService::startAudio()
{
	if (started)
	{
		console::writeLine("VoIP warning: audio is already started.");
		return;
	}

	if (!validateSettings())
		return;

	startAudioInput();
	startAudioOutput();

	if (!mic || !speaker)
	{
		stopAudio();
		return;
	}

	started = true;
	playTimer->start();
}

void VoIPService::stopAudio()
{
	if (playTimer)
		playTimer->stop();

	if (audioInput)
		audioInput->stop();

	if (audioOutput)
		audioOutput->stop();

	audioInput.reset();
	audioOutput.reset();

	mic = nullptr;
	speaker = nullptr;
	started = false;
	audioBuffer.clear();
}

void VoIPService::startAudioInput()
{
	inDevice = QAudioDeviceInfo::defaultInputDevice();

	if (!inDevice.isNull() && !inDevice.isFormatSupported(audioFormat))
	{
		console::writeLine("VoIP Error: input format is not supported by the default input device.");
		return;
	}

	audioInput = std::make_unique<QAudioInput>(inDevice, audioFormat);
	mic = audioInput->start();

	if (!mic)
	{
		console::writeLine("VoIP Error: failed to start audio input.");
		return;
	}

	connect(mic, &QIODevice::readyRead, this, [this]() {
		if (!mic || !socket)
			return;

		const QByteArray data = mic->readAll();
		if (data.isEmpty())
			return;

		const qint64 sent = socket->writeDatagram(data, targetAddress, targetPort);
		if (sent < 0)
			console::writeLine("VoIP Error: failed to send datagram: " + socket->errorString());
	});
}

void VoIPService::startAudioOutput()
{
	outDevice = QAudioDeviceInfo::defaultOutputDevice();

	if (!outDevice.isNull() && !outDevice.isFormatSupported(audioFormat))
	{
		console::writeLine("VoIP Error: output format is not supported by the default output device.");
		return;
	}

	audioOutput = std::make_unique<QAudioOutput>(outDevice, audioFormat);
	speaker = audioOutput->start();

	if (!speaker)
	{
		console::writeLine("VoIP Error: failed to start audio output.");
		return;
	}

	connect(socket, &QUdpSocket::readyRead, this, [this]() {
		while (socket->hasPendingDatagrams())
		{
			QByteArray datagram;
			datagram.resize(static_cast<int>(socket->pendingDatagramSize()));
			socket->readDatagram(datagram.data(), datagram.size());
			audioBuffer.append(datagram);
		}

		resetAudioBufferIfNeeded();
	});
}

bool VoIPService::validateSettings() const
{
	if (!portIsSet)
	{
		console::writeLine("VoIP Error: local port is not set. Use setLocalPort().");
		return false;
	}

	if (targetPort == 0)
	{
		console::writeLine("VoIP Error: target port is not set. Use setTargetPort().");
		return false;
	}

	if (targetAddress.isNull())
	{
		console::writeLine("VoIP Error: target address is not set or invalid. Use setTargetAddress().");
		return false;
	}

	return true;
}

void VoIPService::resetAudioBufferIfNeeded()
{
	if (audioBuffer.size() > MaxBufferedAudio)
	{
		console::writeLine("VoIP warning: audio buffer overflow, dropping buffered audio.");
		audioBuffer.clear();
	}
}

quint16 VoIPService::getLocalPort() const
{
	return localPort;
}

void VoIPService::setLocalPort(quint16 newLocalPort)
{
	if (started)
		stopAudio();

	if (socket->state() != QAbstractSocket::UnconnectedState)
		socket->close();

	const bool ok = socket->bind(QHostAddress::Any,
								 newLocalPort,
								 QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);

	if (!ok)
	{
		portIsSet = false;
		localPort = 0;
		console::writeLine("VoIP Error: bind failed: " + socket->errorString());
		return;
	}

	localPort = socket->localPort();
	portIsSet = true;
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

bool VoIPService::isStarted() const
{
	return started;
}


// void VoIPService::startAudioInput()
// {
// 	inDevice = QAudioDeviceInfo::defaultInputDevice();

// 	audioInput = std::make_unique<QAudioInput>(inDevice, audioFormat);
// 	mic = audioInput->start();

// 	if (mic == nullptr)
// 	{
// 		console::writeLine("[VoIP] Error: Mic invalid.");
// 		return;
// 	}

// 	connect(mic, &QIODevice::readyRead, this, [=] () {
// 		if (targetAddress.isNull() || targetPort == 0)
// 			return;
// 		QByteArray data = mic->readAll();
// 		socket->writeDatagram(data, targetAddress, targetPort);
// 	});
// }

// void VoIPService::startAudioOutput()
// {
// 	connect(socket, &QUdpSocket::readyRead, this, [=] () {
// 		for ( ; socket->hasPendingDatagrams() ; )
// 		{
// 			QByteArray datagram;
// 			datagram.resize(socket->pendingDatagramSize());
// 			socket->readDatagram(datagram.data(), datagram.size());
// 			audioBuffer.append(datagram);
// 		}
// 	});

// 	outDevice = QAudioDeviceInfo::defaultOutputDevice();
// 	audioOutput = std::make_unique<QAudioOutput>(outDevice, audioFormat);
// 	speaker = audioOutput->start();

// 	if (speaker == nullptr)
// 	{
// 		console::writeLine("[VoIP] Error: Speaker invalid.");
// 		return;
// 	}

// 	QTimer* playTimer = new QTimer(this);
// 	connect(playTimer, &QTimer::timeout, this, [=] () {
// 		if (audioBuffer.size() >= 2048)
// 		{
// 			speaker->write(audioBuffer.left(2048));
// 			audioBuffer.remove(0, 2048);
// 		}
// 	});
// 	playTimer->start(20);
// }

// VoIPService::VoIPService(QObject *parent)
// 	: QObject{parent}
// 	, targetPort{}
// {
// 	socket = new QUdpSocket(this);

// 	audioFormat.setSampleRate(48000);
// 	audioFormat.setChannelCount(1);
// 	audioFormat.setSampleSize(16);
// 	audioFormat.setSampleType(QAudioFormat::SignedInt);
// 	audioFormat.setByteOrder(QAudioFormat::LittleEndian);
// 	audioFormat.setCodec("audio/pcm");

// 	if (!inDevice.isFormatSupported(audioFormat))
// 		console::writeLine("[VoIP]: Input audio device format is not supported (audio/pcm).");
// 	if (!outDevice.isFormatSupported(audioFormat))
// 		console::writeLine("[VoIP]: Output audio device format is not supported (audio/pcm).");
// }

// void VoIPService::startAudio()
// {
// 	if (audioStarted) return;

// 	if (portIsset == false)
// 	{
// 		console::writeLine("[VoIP] Error: Local port not set. Use setLocalPort().");
// 		return;
// 	}

// 	startAudioInput();
// 	startAudioOutput();

// 	audioStarted = true;
// }

// quint16 VoIPService::getLocalPort() const
// {
// 	return localPort;
// }

// void VoIPService::setLocalPort(quint16 newLocalPort)
// {
// 	if (socket->state() != QAbstractSocket::UnconnectedState)
// 		socket->close();

// 	bool ok = socket->bind(QHostAddress::Any,
// 						   newLocalPort,
// 						   QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);

// 	if (!ok) console::writeLine("[VoIP]: Bind failed - " + socket->errorString());

// 	localPort = socket->localPort();
// 	portIsset = true;
// }

// quint16 VoIPService::getTargetPort() const
// {
// 	return targetPort;
// }

// void VoIPService::setTargetPort(quint16 newTargetPort)
// {
// 	targetPort = newTargetPort;
// }

// QHostAddress VoIPService::getTargetAddress() const
// {
// 	return targetAddress;
// }

// void VoIPService::setTargetAddress(const QHostAddress &newTargetAddress)
// {
// 	targetAddress = newTargetAddress;
// }

// void VoIPService::setTargetAddress(const QString &newTargetAddress)
// {
// 	targetAddress = QHostAddress(newTargetAddress);
// }
