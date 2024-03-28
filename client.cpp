#include "client.h"
#include "ui_client.h"
#include "network/account.h"
#include "settings/settings_manager.h"
#include "widgets/profile_widget.h"
#include "widgets/advert_widget.h"
#include "widgets/authorization_widget.h"

#include <QMouseEvent>
#include <QDebug>

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

namespace profilewidget
{
	ProfileWidget* getInstance();
}

// static QByteArray swapBytes_2(const QByteArray& data)
// {
// 	QByteArray result(data.size(), Qt::Uninitialized);
// 	char* dst = result.data();
// 	const char* src = data.constData();
// 	const char* end = src + data.size();
// 	for (; src != end; src += 2, dst += 2)
// 	{
// 		dst[0] = src[1];
// 		dst[1] = src[0];
// 	}
// 	return result;
// }

Client::Client(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::Client)
{
	ui->setupUi(this);
	setWindowFlag(Qt::WindowType::FramelessWindowHint);

	acc = new Account(this);
	acc->setInetAddress(SettingsManager::getInstance().getEndPoint());
	acc->start();

	AdvertWidget* advert = new AdvertWidget(QColor(0, 210, 180), QColor(80, 200, 250), this);
	advert->setTitle("Amalgama's Chat is available for free!");
	advert->setDescription("To get started, create an account or sign in.");
	advert->setLinkText("Sign in");
	advert->setLink("https://infotecs.ru/");
	addAdvertPage(advert);

	show();
}

Client::~Client()
{
	delete ui;
}

void Client::openMyProfilePage()
{
	if (!(currentPage == nullptr || currentPage == pw))
		currentPage->hide();

	if (pw != nullptr)
	{
		pw->show();
		currentPage = pw;
	}
	else
	{
		pw = profilewidget::getInstance();
		ui->frame->layout()->addWidget(pw);
		pw->show();
		currentPage = pw;
	}

	pw->setAvatar(QImage("cache/images/users/amalgama.jpg"));
}

void Client::authWindow()
{
	if (authWidget == nullptr)
	{
		authWidget = new AuthorizationWidget(this, true);
	}

	authWidget->move(this->width() / 2 - authWidget->width() / 2, this->height() / 2 - authWidget->height() / 2);
	authWidget->show();
}

void Client::closeEvent(QCloseEvent *)
{
	closePages();
	emit event_close();
	delete this;
}

void Client::closePages()
{
	if (pw != nullptr)
		pw->close();
}

void Client::enableSideButtons()
{
	ui->btnProfile->setEnabled(true);
	ui->btnLogout->setEnabled(true);
	ui->btnSearch->setEnabled(true);
	ui->btnChats->setEnabled(true);
}

void Client::disableSideButtons()
{
	ui->btnProfile->setEnabled(false);
	ui->btnLogout->setEnabled(false);
	ui->btnSearch->setEnabled(false);
	ui->btnChats->setEnabled(false);
}

void Client::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::MouseButton::LeftButton && event->pos().y() < 31)
	{
		lastDragPos = event->globalPos();
		dragging = true;
	}
}

void Client::mouseMoveEvent(QMouseEvent* event)
{
	if (dragging && (event->buttons() & Qt::MouseButton::LeftButton))
	{
		QPoint delta = event->globalPos() - lastDragPos;
		move(pos() + delta);
		lastDragPos = event->globalPos();
	}
}

void Client::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::MouseButton::LeftButton)
	{
		dragging = false;
	}
}

void Client::on_btnProfile_clicked()
{
	openMyProfilePage();
}


void Client::on_btnClose_clicked()
{
	close();
}


void Client::on_btnSize_clicked()
{
	isMaximized = !isMaximized;
	if (isMaximized)
	{
		setWindowState(Qt::WindowMaximized);
		ui->btnSize->setIcon(QIcon(":/r/resources/images/minimize_2.png"));
	}
	else
	{
		setWindowState(Qt::WindowNoState);
		ui->btnSize->setIcon(QIcon(":/r/resources/images/maximize.png"));
	}
}


void Client::on_btnHide_clicked()
{
	setWindowState(Qt::WindowMinimized);
}

void Client::addAdvertPage(QWidget* page)
{
	ui->stackedWidget->addWidget(page);
	ui->stackedWidget->setCurrentIndex(ui->stackedWidget->count() - 1);
}
