#include "console_widget.h"
#include "ui_console_widget.h"
#include "chats_widget.h"
#include "../client.h"
#include "../network/account.h"


namespace console
{
	ConsoleWidget* console = nullptr;
	QStringList buffer;

	void free()
	{
		console = nullptr;
	}

	void show()
	{
		if (console == nullptr)
		{
			console = new ConsoleWidget();
			QObject::connect(console, &ConsoleWidget::event_close, free);
			for (QString line : buffer)
				console->writeLine(line);
		}
		else
		{
			console->activateWindow();
		}
	}

	void writeLine(const QString& text)
	{
		if (console != nullptr)
		{
			console->writeBufferedLine(text);
		}
		else
		{
			buffer.append(text);
		}
	}
}

namespace client
{
	extern Client* window;
}

namespace chatswidget
{
	ChatsWidget* getInstance();
}

ConsoleWidget::ConsoleWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ConsoleWidget)
{
	ui->setupUi(this);
	setStyleSheet("#plainTextEdit { color: rgb(255, 230, 50); background: black; font-family: 'Cascadia Mono'; font-size: 12pt; }");
	ui->plainTextEdit->setWordWrapMode(QTextOption::WordWrap);
	show();
}

ConsoleWidget::~ConsoleWidget()
{
	delete ui;
}

void ConsoleWidget::writeLine(const QString &text)
{
	ui->plainTextEdit->appendPlainText(text);
}

void ConsoleWidget::writeBufferedLine(const QString &text)
{
	console::buffer.append(text);
	ui->plainTextEdit->appendPlainText(text);
}

void ConsoleWidget::on_lineEdit_returnPressed()
{
	QString command = ui->lineEdit->text();
	ui->lineEdit->clear();
	writeBufferedLine("user: " + command);
	QStringList args = command.split(QRegExp("\\s"));
	processCommand(args.takeFirst(), args);
}

void ConsoleWidget::closeEvent(QCloseEvent *)
{
	emit event_close();
	delete this;
}

void ConsoleWidget::showEvent(QShowEvent *)
{
	ui->lineEdit->setFocus();
}

void ConsoleWidget::processCommand(const QString &command, const QStringList& args)
{
	if (command == "help")
	{
		printHelp();
	}
	else if (command == "clear_visual_chat" || command == "cvc")
	{
		chatswidget::getInstance()->clearCurrentChat();
	}
	else if (command == "gpt")
	{
		client::window->createChatGPT();
	}
	else if (command == "set_proxy")
	{
		writeBufferedLine("This command temporary unsupported.");
	}
	else if (command == "test_call_notify" || command == "tcn")
	{
		if (client::window->acc->isConnected())
		{
			const ProfileData* _profile = client::window->acc->getData();
			client::window->showCallNotify(_profile->fname, _profile->avatar);
		}
		else
		{
			client::window->showCallNotify("Niky", QImage());
		}
	}
	else if (command == "hide_call")
	{
		client::window->hideCallNotify();
	}
	else if (command == "receive")
	{
		if (args.isEmpty())
			return;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wc++17-extensions"
		if (uint32 id = args.first().toUInt(); id != 0)
			client::window->acc->forceReceivePacket(id);
#pragma GCC diagnostic pop
	}
	else if (command == "get_chat_id")
	{
		quint64 chatId = client::window->getCurrentChatId();
		writeBufferedLine("Current chat ID = " + QString::number(chatId));
	}
	else if (command == "clear")
	{
		ui->plainTextEdit->clear();
		console::buffer.clear();
	}
	else
	{
		writeBufferedLine("Invalid command: " + command);
	}
}

void ConsoleWidget::printHelp()
{
	writeLine("Commands:");
	writeLine("help                      - Print this page.");
	writeLine("clear_visual_chat         - Clear visual messages in current chat");
	writeLine("gpt                       - Add ChatGPT to chats.");
	writeLine("set_proxy                 - Sets proxy for ChatGPT or other Network Services.");
	writeLine("receive <ID>              - Force simulate receive network packet by ID.");
}
