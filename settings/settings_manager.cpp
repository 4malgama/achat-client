#include "settings_manager.h"


namespace settings
{
	QString hostIp;
	quint16 hostPort = 0;

	quint64 lastUid = 0;

	WebService::ProxyData proxyData;
	QString GPTToken;
}

SettingsManager &SettingsManager::getInstance()
{
	static SettingsManager sm;
	return sm;
}

SettingsManager::SettingsManager()
	: qs("settings.ini", QSettings::defaultFormat())
{
	settings::hostIp = qs.value("host", "127.0.0.1").toString();
	settings::hostPort = qs.value("port", 13050).toInt();
	settings::lastUid = getLastUid();
	settings::proxyData = getProxyData();
	settings::GPTToken = getGPTToken();
}

Network::InetAddress SettingsManager::getEndPoint()
{
	return { .ip = settings::hostIp, .port = settings::hostPort };
}

void SettingsManager::setEndPoint(const Network::InetAddress &endPoint)
{
	qs.setValue("host", endPoint.ip);
	qs.setValue("port", endPoint.port);
}

void SettingsManager::saveAll()
{
	qs.setValue("host", settings::hostIp);
	qs.setValue("port", settings::hostPort);
	setLastUid(settings::lastUid);
	setProxyData(settings::proxyData);
	setGPTToken(settings::GPTToken);
}

quint64 SettingsManager::getLastUid()
{
	qs.beginGroup("UserData");
	settings::lastUid = qs.value("last_uid", 0).toULongLong();
	qs.endGroup();
	return settings::lastUid;
}

void SettingsManager::setLastUid(quint64 uid)
{
	settings::lastUid = uid;
	qs.beginGroup("UserData");
	qs.setValue("last_uid", uid);
	qs.endGroup();
}

WebService::ProxyData SettingsManager::getProxyData()
{
	qs.beginGroup("ProxyData");
	settings::proxyData.ip = qs.value("host").toString();
	settings::proxyData.port = qs.value("port").toInt();
	settings::proxyData.user = qs.value("user").toString();
	settings::proxyData.password = qs.value("password").toString();
	qs.endGroup();
	return settings::proxyData;
}

void SettingsManager::setProxyData(const WebService::ProxyData &proxyData)
{
	qs.beginGroup("ProxyData");
	qs.setValue("host", proxyData.ip);
	qs.setValue("port", proxyData.port);
	qs.setValue("user", proxyData.user);
	qs.setValue("password", proxyData.password);
	qs.endGroup();
}

QString SettingsManager::getGPTToken()
{
	qs.beginGroup("GPTService");
	settings::GPTToken = qs.value("token").toString();
	qs.endGroup();
	return settings::GPTToken;
}

void SettingsManager::setGPTToken(const QString &token)
{
	qs.beginGroup("GPTService");
	qs.setValue("token", token);
	qs.endGroup();
}
