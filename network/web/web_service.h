#ifndef WEBSERVICE_H
#define WEBSERVICE_H

#include <QObject>
#include <QEventLoop>
#include "request_builder.h"

class QNetworkAccessManager;

class WebService : public QObject
{
	Q_OBJECT
public:
	struct ProxyData
	{
		quint16 port;
		QString ip;
		QString user;
		QString password;
	};

	explicit WebService(QObject *parent = nullptr);

	QByteArray get(const QString& url);
	QByteArray get(const RequestBuilder& builder);
	QByteArray post(const RequestBuilder& builder, const QByteArray& data);

	void setProxy(const QString& ip, quint16 port);
	void setProxy(const QString& ip, quint16 port, const QString& user, const QString& password);
	void resetProxy();

protected:
	QByteArray _get(const QNetworkRequest* req);
	QByteArray _post(const QNetworkRequest* req, const QByteArray& data);

	QNetworkAccessManager* manager;
	QEventLoop loop;
};

#endif // WEBSERVICE_H
