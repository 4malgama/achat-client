#include "client.h"
#include "ui_client.h"
#include "application.h"
#include "network/account.h"
#include "settings/settings_manager.h"
#include "widgets/profile_widget.h"
#include "widgets/chats_widget.h"
#include "widgets/search_widget.h"
#include "widgets/advert_widget.h"
#include "widgets/chat_message_widget.h"
#include "widgets/chat_row_widget.h"
#include "widgets/search_result_widget.h"
#include "widgets/authorization_widget.h"
#include "widgets/message_widget.h"
#include "resource_manager/resource_manager.h"
#include "types/classes.h"
#include "utils/image_utils.h"

#include <QMouseEvent>
#include <QHash>
#include <QVariant>

#include <QDebug>

namespace app
{
	extern Application* a;
	extern bool debugMode;
}

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
			window->show();
			window->activateWindow();
		}
	}
}

namespace profilewidget
{
	ProfileWidget* getInstance();
}

namespace chatswidget
{
	ChatsWidget* getInstance();
}

namespace searchwidget
{
	SearchWidget* getInstance();
}

namespace console
{
	void show();
}

Client::Client(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::Client)
{
	ui->setupUi(this);
	setWindowFlag(Qt::WindowType::FramelessWindowHint);
	setWindowTitle(tr("Main"));

	if (!app::debugMode)
		ui->btnConsole->hide();

	ui->btnConsole->setEnabled(app::debugMode);

	acc = new Account(this);
	acc->setInetAddress(SettingsManager::getInstance().getEndPoint());

	AdvertWidget* advert = new AdvertWidget(QColor(0, 210, 180), QColor(80, 200, 250), this);
	advert->setTitle(tr("Amalgama's Chat is available for free!"));
	advert->setDescription(tr("To get started, create an account or sign in."));
	advert->setLinkText(tr("Sign in"));
	advert->setLink("https://infotecs.ru/");
	addAdvertPage(advert);

	if (ResourceManager& rm = ResourceManager::instance(); rm.isLoaded() == false)
	{
		connect(&rm, &ResourceManager::event_finish, this, [this] {
			acc->start();
		});
	}
	else
	{
		acc->start();
	}

	pw = profilewidget::getInstance();
	ui->frame->layout()->addWidget(pw);

	cw = chatswidget::getInstance();
	ui->frame->layout()->addWidget(cw);

	sw = searchwidget::getInstance();
	ui->frame->layout()->addWidget(sw);

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

	pw->show();
	currentPage = pw;

	QImage avatar = ResourceManager::instance().getAvatar();
	if (avatar.isNull())
		avatar = ImageUtils::GetImageFromName(acc->getData()->fname);
	pw->setAvatar(avatar);
	setWindowTitle(tr("Profile"));
}

void Client::openChatsPage()
{
	if (!(currentPage == nullptr || currentPage == cw))
		currentPage->hide();

	cw->show();
	currentPage = cw;

	setWindowTitle(tr("Chats"));
}

void Client::openSearchPage()
{
	if (!(currentPage == nullptr || currentPage == sw))
		currentPage->hide();

	sw->show();
	currentPage = sw;
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

void Client::closeAuthWindow()
{
	if (authWidget != nullptr)
	{
		authWidget->close(); //it is equals to delete
		authWidget = nullptr;
	}
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
	if (cw != nullptr)
		cw->close();
}

void Client::showMessage(const QString& text, quint8 icon)
{
	MessageWidget* mw = new MessageWidget(this);
	mw->icon = (icon >= MessageWidget::UNKNOWN ? MessageWidget::UNKNOWN : ((MessageWidget::MessageIcon)icon));
	mw->message = text;
	mw->show();
}

void Client::addMessageToChat(quint64 chatId, ChatMessage* message)
{
	if (cw == nullptr)
		return;

	bool isMine = message->user.uid == acc->getData()->uid;
	cw->addMessageToChat(chatId, message, isMine);

	if (isMine) return;

	const ChatData* chatData = cw->getChatData(chatId);
	if (chatData != nullptr)
	{
		QIcon icon = QIcon(QPixmap::fromImage(ImageUtils::CropImageToCircle(chatData->data.user.avatar)));
		app::a->message(icon, message->user.sname + " " + message->user.fname, message->content);
	}
}

void Client::initChats(const QList<InitChatData> &chats)
{
	if (cw != nullptr)
	{
		cw->addChats(chats);
	}
}

void Client::initMessages(quint64 chatId, const QList<ChatMessage>& messages)
{
	if (cw != nullptr)
	{
		cw->initMessages(chatId, messages);
	}
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
	//close();
	hide();
	app::a->message(tr("The application continues to work in the background"));
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

void Client::setProfileData(const QHash<QString, QVariant>& profileInfo)
{
	if (pw != nullptr)
	{
		pw->accountLink = profileInfo.value("login").toString();
		pw->name1 = profileInfo.value("first_name").toString();
		pw->name2 = profileInfo.value("sur_name").toString();
		pw->name3 = profileInfo.value("patronymic").toString();
		pw->post = profileInfo.value("post").toString();
		pw->description = profileInfo.value("description").toString();
		pw->updateData();

		// For generate new colored avatar
		{
			QImage avatar = ResourceManager::instance().getAvatar();
			if (avatar.isNull())
				avatar = ImageUtils::GetImageFromName(acc->getData()->fname);
			pw->setAvatar(avatar);
		}
	}
}

void Client::openChat(quint64 id)
{
	if (cw != nullptr)
	{
		cw->openChat(id);
		openChatsPage();
	}
}

void Client::createChatGPT()
{
	if (cw != nullptr)
	{
		cw->addChatGPT();
	}
}

void Client::resizeEvent(QResizeEvent* event)
{
	emit event_resize(event->size());
}

void Client::on_btnChats_clicked()
{
	openChatsPage();
}


void Client::on_btnConsole_clicked()
{
	console::show();
}


void Client::on_btnSearch_clicked()
{
	openSearchPage();
}

