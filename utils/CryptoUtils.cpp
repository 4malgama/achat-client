#include <QCryptographicHash>
#include <QFile>


namespace crypto
{
	QString MD5(const QString& data)
	{
		return QCryptographicHash::hash(data.toUtf8(), QCryptographicHash::Md5).toHex();
	}

	QByteArray MD5(const QByteArray& data)
	{
		return QCryptographicHash::hash(data, QCryptographicHash::Md5);
	}

	QString FileHash(const QString& filePath)
	{
		QFile file(filePath);
		if (file.open(QIODevice::ReadOnly))
		{
			return QCryptographicHash::hash(file.readAll(), QCryptographicHash::Algorithm::Md5).toHex();
			file.close();
		}
		return QString();
	}
}
