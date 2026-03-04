#include "application.h"
#include <QLocale>
#include <QTranslator>
#include <settings/settings_manager.h>
#include <QFile>
#include <QMessageBox>
#include <QCommandLineParser>
#ifdef WIN32
#include <windows.h>
#endif


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

#ifndef WIN32
static void signalHandler(int sigsum)
{
	QFile log("crash-log.txt");
	if (log.open(QIODevice::WriteOnly))
	{
		log.write(QByteArray("Crash signal: ") + QByteArray::number(sigsum));
		log.close();
	}
	qApp->exit(sigsum);
}
#else
static QString ExceptionCodeToString(DWORD code)
{
	switch (code)
	{
	case EXCEPTION_ACCESS_VIOLATION: return "ACCESS_VIOLATION";
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: return "ARRAY_BOUNDS_EXCEEDED";
	case EXCEPTION_BREAKPOINT: return "BREAKPOINT";
	case EXCEPTION_DATATYPE_MISALIGNMENT: return "DATATYPE_MISALIGNMENT";
	case EXCEPTION_FLT_DIVIDE_BY_ZERO: return "FLT_DIVIDE_BY_ZERO";
	case EXCEPTION_ILLEGAL_INSTRUCTION: return "ILLEGAL_INSTRUCTION";
	case EXCEPTION_INT_DIVIDE_BY_ZERO: return "INT_DIVIDE_BY_ZERO";
	case EXCEPTION_STACK_OVERFLOW: return "STACK_OVERFLOW";
	default: return "UNKNOWN";
	}
}

LONG WINAPI _UnhandledExceptionFilter(EXCEPTION_POINTERS* ExceptionInfo)
{
	QFile log("crash-log-windows.txt");
	if (log.open(QIODevice::WriteOnly))
	{
		log.write(QByteArray("Crash code: ") + QByteArray::number((quint32)ExceptionInfo->ExceptionRecord->ExceptionCode, 16));
		log.write(QByteArray("\nCrash string: ") + ExceptionCodeToString(ExceptionInfo->ExceptionRecord->ExceptionCode).toUtf8());

		CONTEXT* ctx = ExceptionInfo->ContextRecord;
		EXCEPTION_RECORD* record = ExceptionInfo->ExceptionRecord;

#ifndef _WIN64
		log.write("\n=== REGISTERS ===\n");
		log.write("EIP: 0x" + QByteArray::number((quint32)ctx->Eip, 16) + "\n");
		log.write("ESP: 0x" + QByteArray::number((quint32)ctx->Esp, 16) + "\n");
		log.write("EBP: 0x" + QByteArray::number((quint32)ctx->Ebp, 16) + "\n");
		log.write("EAX: 0x" + QByteArray::number((quint32)ctx->Eax, 16) + "\n");
		log.write("EBX: 0x" + QByteArray::number((quint32)ctx->Ebx, 16) + "\n");
		log.write("ECX: 0x" + QByteArray::number((quint32)ctx->Ecx, 16) + "\n");
		log.write("EDX: 0x" + QByteArray::number((quint32)ctx->Edx, 16) + "\n");

		log.write("\n=== EXCEPTION INFO ===\n");
		log.write("Code: 0x" + QByteArray::number((quint32)record->ExceptionCode, 16) + "\n");
		log.write("Flags: 0x" + QByteArray::number((quint32)record->ExceptionFlags, 16) + "\n");
		log.write("Address: 0x" + QByteArray::number((quint32)record->ExceptionAddress, 16) + "\n");

		log.write("Parameters count: " + QByteArray::number((quint32)record->NumberParameters) + "\n");

		for (ULONG i = 0; i < record->NumberParameters; ++i)
		{
			log.write("Param[" + QByteArray::number((quint32)i) + "]: 0x" +
					  QByteArray::number((quint64)record->ExceptionInformation[i], 16) + "\n");
		}
#else
		log.write("\n=== REGISTERS ===\n");
		log.write("RIP: 0x" + QByteArray::number((quint64)ctx->Rip, 16) + "\n");
		log.write("RSP: 0x" + QByteArray::number((quint64)ctx->Rsp, 16) + "\n");
		log.write("RBP: 0x" + QByteArray::number((quint64)ctx->Rbp, 16) + "\n");
		log.write("RAX: 0x" + QByteArray::number((quint64)ctx->Rax, 16) + "\n");
		log.write("RBX: 0x" + QByteArray::number((quint64)ctx->Rbx, 16) + "\n");
		log.write("RCX: 0x" + QByteArray::number((quint64)ctx->Rcx, 16) + "\n");
		log.write("RDX: 0x" + QByteArray::number((quint64)ctx->Rdx, 16) + "\n");

		log.write("\n=== EXCEPTION INFO ===\n");
		log.write("Code: 0x" + QByteArray::number((quint64)record->ExceptionCode, 16) + "\n");
		log.write("Flags: 0x" + QByteArray::number((quint64)record->ExceptionFlags, 16) + "\n");
		log.write("Address: 0x" + QByteArray::number((quint64)record->ExceptionAddress, 16) + "\n");

		log.write("Parameters count: " + QByteArray::number((quint32)record->NumberParameters) + "\n");

		for (ULONG i = 0; i < record->NumberParameters; ++i)
		{
			log.write("Param[" + QByteArray::number((quint64)i) + "]: 0x" +
					  QByteArray::number((quint64)record->ExceptionInformation[i], 16) + "\n");
		}
#endif
	}

	return EXCEPTION_EXECUTE_HANDLER;
}
#endif

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

#ifndef WIN32
	signal(SIGSEGV, signalHandler);
#else
	SetUnhandledExceptionFilter(_UnhandledExceptionFilter);
#endif

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
