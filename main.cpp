#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <settings/settings_manager.h>

namespace client
{
	void show();
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QSettings::setDefaultFormat(QSettings::IniFormat);

	qApp->setOrganizationName("Amalgama");

	QTranslator translator;
	const QStringList uiLanguages = QLocale::system().uiLanguages();
	for (const QString &locale : uiLanguages) {
		const QString baseName = "AdvancedChatClient_" + QLocale(locale).name();
		if (translator.load(":/i18n/" + baseName)) {
			a.installTranslator(&translator);
			break;
		}
	}

	client::show();

	int retCode = a.exec();

	SettingsManager::getInstance().saveAll();

	return retCode;
}
