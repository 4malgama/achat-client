#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QSettings>
#include "../network/network.h"
#include "../network/web/web_service.h"

class SettingsManager
{
	QSettings qs;

public:
	static SettingsManager& getInstance();

	SettingsManager();

	Network::InetAddress getEndPoint();
	void setEndPoint(const Network::InetAddress& endPoint);

	quint64 getLastUid();
	void setLastUid(quint64 uid);

	WebService::ProxyData getProxyData();
	void setProxyData(const WebService::ProxyData& proxyData);

	QString getGPTToken();
	void setGPTToken(const QString& token);

	void saveAll();
};

#endif // SETTINGSMANAGER_H
