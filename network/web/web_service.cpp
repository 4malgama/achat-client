#include "web_service.h"
#include <QNetworkAccessManager>
#include <QNetworkProxy>
#include <QNetworkReply>

WebService::WebService(QObject *parent)
	: QObject{parent}
{
	manager = new QNetworkAccessManager;
	manager->setProxy(QNetworkProxy::NoProxy);
	connect(manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
}

QByteArray WebService::get(const QString &url)
{
	QNetworkRequest req(url);
	return _get(&req);
}

QByteArray WebService::get(const RequestBuilder &builder)
{
	QNetworkRequest req = builder.build();
	return _get(&req);
}

QByteArray WebService::post(const RequestBuilder &builder, const QByteArray &data)
{
	QNetworkRequest req = builder.build();
	return _post(&req, data);
}

void WebService::setProxy(const QString &ip, quint16 port)
{
	QNetworkProxy proxy;
	proxy.setHostName(ip);
	proxy.setPort(port);
	proxy.setType(QNetworkProxy::HttpProxy);
	manager->setProxy(proxy);
}

void WebService::setProxy(const QString &ip, quint16 port, const QString &user, const QString &password)
{
	QNetworkProxy proxy;
	proxy.setHostName(ip);
	proxy.setPort(port);
	proxy.setUser(user);
	proxy.setPassword(password);
	proxy.setType(QNetworkProxy::HttpProxy);
	manager->setProxy(proxy);
}

void WebService::resetProxy()
{
	manager->setProxy(QNetworkProxy::NoProxy);
}

QByteArray WebService::_get(const QNetworkRequest *req)
{
	QNetworkReply* reply = manager->get(*req);
	do { loop.exec(); } while (reply->isFinished() == false);
	QByteArray data = reply->readAll();
	delete reply;
	return data;
}

QByteArray WebService::_post(const QNetworkRequest *req, const QByteArray &data)
{
	QNetworkReply* reply = manager->post(*req, data);
	do { loop.exec(); } while (reply->isFinished() == false);
	QByteArray result = reply->readAll();
	delete reply;
	return result;
}
