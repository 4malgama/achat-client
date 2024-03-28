#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <settings/settings_manager.h>
#include <QFile>

namespace client
{
	void show();
}

namespace unload { void free(); }

static void setDarkTheme()
{
	QFile styleSheetFile(":/r/themes/dark.qss");
	if (styleSheetFile.open(QFile::ReadOnly))
	{
		QString styleSheet = QLatin1String(styleSheetFile.readAll());
		qApp->setStyleSheet(styleSheet);
		styleSheetFile.close();
	}
	else
	{
		qApp->setStyleSheet("");
	}
}

static void _main_end()
{
	unload::free();

	SettingsManager::getInstance().saveAll();
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

	setDarkTheme();

	client::show();

	int retCode = a.exec();

	_main_end();

	return retCode;
}
