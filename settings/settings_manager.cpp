#include "settings_manager.h"


namespace settings
{
	QString hostIp;
	quint16 hostPort = 0;
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
}
