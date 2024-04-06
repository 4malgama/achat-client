#include "settings_manager.h"


namespace settings
{
	QString hostIp;
	quint16 hostPort = 0;

	quint64 lastUid = 0;
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
}

Network::InetAddress SettingsManager::getEndPoint()
{
	return (Network::InetAddress) { .ip = settings::hostIp, .port = settings::hostPort };
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