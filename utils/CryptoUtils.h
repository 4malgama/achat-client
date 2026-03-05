#ifndef CRYPTOUTILS_H
#define CRYPTOUTILS_H

#include <QString>
#include <QByteArray>

namespace crypto
{
	QString md5(const QString& data);
	QByteArray md5(const QByteArray& data);

	QString sha256(const QString& data);
	QByteArray sha256(const QByteArray& data);

	QString fileMD5(const QString& filePath);
	QString fileSHA256(const QString& filePath);
}

#endif // CRYPTOUTILS_H
