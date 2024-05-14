#ifndef CERTIFICATION_MANAGER_H
#define CERTIFICATION_MANAGER_H


#include <QByteArray>

class CertificationManager
{
public:
	static bool verifyCertificate(const QByteArray& data);
};

#endif // CERTIFICATION_MANAGER_H
