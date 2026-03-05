#include <QCryptographicHash>
#include <QFile>


namespace crypto
{
	QString md5(const QString& data)
	{
		return QCryptographicHash::hash(
			data.toUtf8(),
			QCryptographicHash::Md5
		).toHex();	}

	QByteArray md5(const QByteArray& data)
	{
		return QCryptographicHash::hash(
			data,
			QCryptographicHash::Md5
		);
	}

	QString sha256(const QString& data)
	{
		return QCryptographicHash::hash(
			data.toUtf8(),
			QCryptographicHash::Sha256
		).toHex();
	}

	QByteArray sha256(const QByteArray& data)
	{
		return QCryptographicHash::hash(
			data,
			QCryptographicHash::Sha256
		);
	}

	// QString fileMD5(const QString& filePath)
	// {
	// 	QFile file(filePath);
	// 	if (file.open(QIODevice::ReadOnly))
	// 	{
	// 		QString hash = QCryptographicHash::hash(file.readAll(), QCryptographicHash::Algorithm::Md5).toHex();
	// 		file.close();
	// 		return hash;
	// 	}
	// 	return QString();
	// }

	QString fileMD5(const QString& filePath)
	{
		QFile file(filePath);

		if (!file.exists() || !file.open(QIODevice::ReadOnly))
			return QString();

		QCryptographicHash hash(QCryptographicHash::Md5);

		while (!file.atEnd())
			hash.addData(file.read(8192));

		return hash.result().toHex();
	}

	QString fileSHA256(const QString& filePath)
	{
		QFile file(filePath);

		if (!file.exists() || !file.open(QIODevice::ReadOnly))
			return QString();

		QCryptographicHash hash(QCryptographicHash::Sha256);

		while (!file.atEnd())
			hash.addData(file.read(8192));

		return hash.result().toHex();
	}
}
