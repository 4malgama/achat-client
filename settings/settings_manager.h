#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QSettings>
#include "../network/network.h"

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

	void saveAll();
};

#endif // SETTINGSMANAGER_H
