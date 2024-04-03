#include <QCryptographicHash>
#include <QFile>


namespace crypto
{
    QString PasswordHash(const QString& password)
    {
        return QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Algorithm::Md5).toHex();
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