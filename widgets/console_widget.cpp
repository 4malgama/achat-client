#include "console_widget.h"
#include "ui_console_widget.h"
#include "chats_widget.h"


namespace console
{
	ConsoleWidget* console = nullptr;

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
			console->writeLine(text);
		}
	}
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
	setStyleSheet("#plainTextEdit { color: rgb(0, 255, 0); background: black; font-family: 'Cascadia Mono'; font-size: 12pt; }");
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

void ConsoleWidget::on_lineEdit_returnPressed()
{
	QString command = ui->lineEdit->text();
	ui->lineEdit->clear();
	writeLine("user: " + command);
	processCommand(command.trimmed());
}

void ConsoleWidget::closeEvent(QCloseEvent *)
{
	emit event_close();
	delete this;
}

void ConsoleWidget::processCommand(const QString &command)
{
	if (command == "help")
	{
		printHelp();
	}
	else if (command == "clear_visual_chat" || command == "cvc")
	{
		chatswidget::getInstance()->clearCurrentChat();
	}
}

void ConsoleWidget::printHelp()
{
	writeLine("Commands:");
	writeLine("help              - Print this page.");
	writeLine("clear_visual_chat - Clear visual messages in current chat");
}
