#include "application.h"
#include <QLocale>
#include <QTranslator>
#include <settings/settings_manager.h>
#include <QFile>
#include <QMessageBox>
#include <QCommandLineParser>


namespace client
{
	void show();
}

namespace unload { void free(); }
namespace resourcemanager { void load(); }
namespace aes { void init(); }

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

namespace app
{
	Application* a = nullptr;
	bool debugMode = false;
	QString logPath;
}

int main(int argc, char *argv[])
{
	QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	Application a(argc, argv);
	app::a = &a;
	QSettings::setDefaultFormat(QSettings::IniFormat);

	{
		QCommandLineParser parser;
		parser.setApplicationDescription("AChat-Client");
		parser.addHelpOption();
		parser.addVersionOption();

		QCommandLineOption debug_opt("debug", "Starts application with debug mode.");
		QCommandLineOption logFile_opt(QStringList() << "L" << "log-file", "Output log file.");

		parser.addOption(debug_opt);
		parser.addOption(logFile_opt);

		parser.process(a);

		app::debugMode = parser.isSet(debug_opt);
		app::logPath = parser.value(logFile_opt);
	}

	resourcemanager::load();

	a.setOrganizationName("Amalgama");
	a.setApplicationName("AChat Client");
	a.setApplicationDisplayName("AChat Client");

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

	aes::init();

	client::show();

	int retCode = a.exec();

	_main_end();

	if (retCode != 0)
	{
		QMessageBox::critical(nullptr, "Error", "The program exited with error.\nError code: " + QString::number(retCode));
	}

	return retCode;
}
