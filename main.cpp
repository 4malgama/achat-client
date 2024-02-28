#include <QApplication>
#include <QLocale>
#include <QTranslator>

namespace client
{
	void show();
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

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

	return a.exec();
}
