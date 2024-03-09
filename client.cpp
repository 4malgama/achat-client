#include "client.h"
#include "ui_client.h"
#include "network/account.h"
#include "settings/settings_manager.h"

#include <cmath>

namespace client
{
	Client* window = nullptr;

	void close()
	{
		window = nullptr;
	}

	void show()
	{
		if (window == nullptr)
		{
			window = new Client;
			QObject::connect(window, &Client::event_close, close);
		}
		else
		{
			window->activateWindow();
		}
	}
}

Client::Client(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::Client)
{
	ui->setupUi(this);
	acc = new Account(this);
	acc->setInetAddress(SettingsManager::getInstance().getEndPoint());
	show();
}

Client::~Client()
{
	delete ui;
}

void Client::closeEvent(QCloseEvent *)
{
	emit event_close();
	delete this;
}

