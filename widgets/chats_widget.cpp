#include "chats_widget.h"
#include "ui_chats_widget.h"
#include "chat_message_widget.h"
#include "chat_row_widget.h"
#include "../types/classes.h"
#include <QHBoxLayout>


namespace chatswidget
{
	ChatsWidget* w = nullptr;

	void close()
	{
		w = nullptr;
	}

	ChatsWidget* getInstance()
	{
		if (w == nullptr)
		{
			w = new ChatsWidget();
			QObject::connect(w, &ChatsWidget::event_close, close);
		}
		return w;
	}
}

ChatsWidget::ChatsWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ChatsWidget)
{
	ui->setupUi(this);

	ui->btnAttach->setIcon(QIcon(":/r/resources/images/attach.png"));
	ui->btnAttach->setIconSize({16, 16});
	ui->btnAttach->setMinimumSize(30, 30);

	ui->btnSend->setIcon(QIcon(":/r/resources/images/send.png"));
	ui->btnSend->setIconSize({16, 16});
	ui->btnSend->setMinimumSize(30, 30);

	ui->txtMessage->setMinimumHeight(30);
	ui->txtMessage->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	connect(ui->txtMessage, &QPlainTextEdit::textChanged, this, &ChatsWidget::onTextMessageChanged);

	ui->txtMessage->setFocus();
	hide();
}

ChatsWidget::~ChatsWidget()
{
	delete ui;
}

void ChatsWidget::addChats(const QList<InitChatData> &chats)
{
	for (const InitChatData& chat : chats)
	{
		if (this->chats.contains(chat.id) == false)
		{
			ChatRowWidget* wgt = new ChatRowWidget(this, chat.user.avatar, chat.user.sname + " " + chat.user.fname, chat.user.post);
			this->chats.insert(chat.id, {
								   .data = chat
							   });
			ui->verticalLayout_2->insertWidget(0, wgt);
		}
	}
}

void ChatsWidget::addMessageToCurrentChat(ChatMessageWidget *message)
{
	message->setParent(this);
	if (message->isMine())
	{
		QSpacerItem* item = new QSpacerItem(1, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
		QHBoxLayout* layout = new QHBoxLayout;
		layout->addItem(item);
		layout->addWidget(message);
		ui->msgLayout->addLayout(layout);
	}
	else
	{
		ui->msgLayout->addWidget(message);
	}
}

void ChatsWidget::closeEvent(QCloseEvent *)
{
	emit event_close();
	delete this;
}

void ChatsWidget::onTextMessageChanged()
{
	int countLines = 0;
	QString newText = ui->txtMessage->toPlainText();

	for (int i = 0; i < newText.length(); i++)
		countLines += newText[i] == '\n';

	int height = qMax(30, qMin(120, 30 + countLines * 20));
	ui->txtMessage->setMaximumHeight(height);
	ui->txtMessage->setMinimumHeight(height);
}
