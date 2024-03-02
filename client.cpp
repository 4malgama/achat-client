#include "client.h"
#include "ui_client.h"
#include "network/account.h"
#include "settings/settings_manager.h"

#include <cstdio>

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
	acc->start();
	// {
	// 	struct
	// 	{
	// 		quint16 id;
	// 		quint16 length;
	// 		wchar_t login[4];
	// 	} packet = { _byteswap_ushort(100), _byteswap_ushort(3), L"AAA" };

	// 	for (int i = 0; i < 4; i++)
	// 		packet.login[i] = _byteswap_ushort(packet.login[i]);

	// 	QByteArray data;
	// 	data.setRawData((char*)&packet, sizeof(packet));

	// 	QByteArray test("\x00\x64\x00\x03\x00\x41\x00\x41\x00\x41\x00\x00", 12);
	// }
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

