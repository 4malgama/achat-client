#include "client.h"
#include "ui_client.h"


namespace client
{
	Client* w = nullptr;

	void close()
	{
		w = nullptr;
	}

	void show()
	{
		if (w == nullptr)
		{
			w = new Client;
			QObject::connect(w, &Client::event_close, close);
		}
		else
		{
			w->activateWindow();
		}
	}
}

Client::Client(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::Client)
{
	ui->setupUi(this);
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

