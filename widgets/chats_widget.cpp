#include "chats_widget.h"
#include "ui_chats_widget.h"
#include "chat_message_widget.h"
#include "chat_row_widget.h"
#include "../types/classes.h"
#include "../client.h"
#include "../network/account.h"
#include <QHBoxLayout>
#include <QScrollBar>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QDir>


#define MAX_ATTACH_FILE_SIZE 1024 * 1024 * 10	//10Mb


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

namespace client { extern Client* window; }

ChatsWidget::ChatsWidget(QWidget *parent) :
	QWidget(parent),
	MAX_FILE_SIZE(MAX_ATTACH_FILE_SIZE),
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
	connect(ui->btnSend, &QAbstractButton::clicked, this, &ChatsWidget::onSendClicked);
	connect(ui->btnAttach, &QAbstractButton::clicked, this, &ChatsWidget::onAttachClicked);

	ui->txtMessage->setFocus();
	ui->btnAttach->hide();
	ui->btnSend->hide();
	ui->txtMessage->hide();
	ui->frame->hide();
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
			connect(wgt, &ChatRowWidget::clicked, this, [this, wgt, chat] {
				if (wgt == selectedRowChat)
					return;
				if (!this->chats.contains(chat.id))
					return;
				this->selectedChat = &this->chats[chat.id];
				if (selectedRowChat != nullptr)
					selectedRowChat->setSelected(false);
				wgt->setSelected(true);
				selectedRowChat = wgt;
				clearLayout(ui->msgLayout);
				ui->btnAttach->show();
				ui->btnSend->show();
				ui->txtMessage->show();
				resetAttachments();
				if (!selectedChat->initialized)
				{
					client::window->acc->requestMessages(chat.id);
				}
				else
				{
					initMessages(selectedChat->data.id, selectedChat->messages);
				}

			});
			this->chats.insert(chat.id, {
								   .initialized = false,
								   .data = chat,
								   .messages = QList<ChatMessage>()
							   });
			ui->verticalLayout_2->insertWidget(0, wgt);
		}
	}
}

void ChatsWidget::addMessageToCurrentChat(ChatMessageWidget *message)
{
	if (selectedChat == nullptr)
		return;

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

void ChatsWidget::initMessages(quint64 chatId, const QList<ChatMessage> &messages)
{
	if (chats.contains(chatId))
	{
		chats[chatId].messages = messages;
		chats[chatId].initialized = true;

		for (const ChatMessage& message : messages)
		{
			ChatMessageWidget* wgt = new ChatMessageWidget(this, message.user.uid == client::window->acc->getData()->uid);
			wgt->setText(message.content);
			wgt->setDateTime(QDateTime::fromSecsSinceEpoch(message.timestamp));
			wgt->setAttachments(message.attachments);
			addMessageToCurrentChat(wgt);
		}

		QTimer::singleShot(200, this, [this] {
			QScrollBar* scrollBar = ui->scrollMessages->verticalScrollBar();
			scrollBar->setValue(scrollBar->maximum());
		});
	}
}

void ChatsWidget::addMessageToChat(quint64 chatId, ChatMessage* message, bool isMine)
{
	if (chats.contains(chatId) && message != nullptr)
	{
		ChatData& chat = chats[chatId];

		if (message->replyId)
		{
			ChatMessage& reply = *std::find_if(chat.messages.begin(), chat.messages.end(), [message] (ChatMessage& m) -> bool {
				return message->replyId == m.id;
			});
			message->replyMsg = &reply;
		}

		if (message->forwardId)
		{
			ChatMessage& fwd = *std::find_if(chat.messages.begin(), chat.messages.end(), [message] (ChatMessage& m) -> bool {
				return message->forwardId == m.id;
			});
			message->forwardMsg = &fwd;
		}

		chat.messages.append(*message);

		if (selectedChat->data.id == chat.data.id)
		{
			ChatMessageWidget* messageWgt = new ChatMessageWidget(this, isMine);
			messageWgt->setText(message->content);
			messageWgt->setDateTime(QDateTime::fromSecsSinceEpoch(message->timestamp));
			messageWgt->setAttachments(message->attachments);
			addMessageToCurrentChat(messageWgt);
		}
	}
}

const ChatData* ChatsWidget::getChatData(quint64 chatId) const
{
	if (!chats.contains(chatId))
		return nullptr;

	return &chats.constFind(chatId).value();
}

void ChatsWidget::closeEvent(QCloseEvent *)
{
	emit event_close();
	delete this;
}

void ChatsWidget::showEvent(QShowEvent *)
{
	//deptecated
}

void ChatsWidget::onTextMessageChanged()
{
	int countLines = 0;
	QString newText = ui->txtMessage->toPlainText();

	for (int i = 0; i < newText.length(); i++)
		countLines += newText[i] == '\n';

	int height = qMax(30, qMin(120, 30 + countLines * 20));
	ui->txtMessage->setFixedHeight(height);
}

void ChatsWidget::onSendClicked()
{
	QString text = ui->txtMessage->toPlainText().trimmed();
	if (text.isEmpty())
		return;

	ui->txtMessage->clear();

	if (selectedChat != nullptr && chats.contains(selectedChat->data.id))
	{
		QJsonObject json;
		json.insert("content", text);

		// start files
		QJsonArray jsonAttachments;
		for (const QString& path : qAsConst(attachments))
		{
			QFile file(path);
			if (file.open(QIODevice::ReadOnly))
			{
				QString fileName = file.fileName();
				fileName = fileName.mid(fileName.lastIndexOf('/') + 1);
				QByteArray fileData = file.readAll();
				if (fileData.isEmpty())
					continue;

				QJsonObject jsonAttachment;
				jsonAttachment.insert("name", fileName);
				jsonAttachment.insert("data", QString(fileData.toBase64()));
				jsonAttachments.append(jsonAttachment);
				file.close();
			}
			else
			{
				client::window->showMessage(tr("Some file(s) can not be open."), 1);
				return;
			}
		}
		json.insert("attachments", jsonAttachments);
		// end files

		QByteArray data = QJsonDocument(json).toJson(QJsonDocument::Compact);
		client::window->acc->sendMessage(selectedChat->data.id, data);
		attachments.clear();
		resetAttachments();
	}
}

void ChatsWidget::onAttachClicked()
{
	QStringList files = QFileDialog::getOpenFileNames(this, tr("Select file"), QDir::homePath());

	for (QString& filePath : files)
	{
		QFileInfo fileInfo(filePath);

		if (fileInfo.exists() && fileInfo.isReadable())
		{
			if (fileInfo.size() > MAX_ATTACH_FILE_SIZE || fileInfo.size() == 0)
			{
				client::window->showMessage(tr("The file you selected is too large!\nMaximum file size: %1 Kb.").arg(MAX_FILE_SIZE / 1024), 2);
				return;
			}

			if (attachments.size() >= 10)
			{
				client::window->showMessage(tr("The maximum number of files should not exceed 10."), 1);
				return;
			}

			addAttachment(fileInfo);
		}
	}
}

void ChatsWidget::clearLayout(QLayout* l)
{
	if (l == nullptr)
		return;

	for ( ; l->count() > 1 ; )
	{
		QLayoutItem* item = l->takeAt(0);
		QWidget* wgt = item->widget();
		if (item->spacerItem())
			continue;
		if (wgt != nullptr)
		{
			wgt->deleteLater();
		}
		else
		{
			clearLayout(item->layout());
		}
	}
}

void ChatsWidget::addAttachment(const QFileInfo& info)
{
	if (attachments.isEmpty())
		ui->frame->show();
	attachments.append(info.absoluteFilePath());
	QLabel* lbl = ui->frame->findChild<QLabel*>("attach" + QString::number(attachments.size()));
	if (lbl == nullptr)
		return;
	lbl->setText(QString("[%1] %2").arg(info.suffix().toUpper(), info.baseName()));
}

void ChatsWidget::resetAttachments()
{
	attachments.clear();
	ui->frame->hide();
	ui->attach1->clear();
	ui->attach2->clear();
	ui->attach3->clear();
	ui->attach4->clear();
	ui->attach5->clear();
	ui->attach6->clear();
	ui->attach7->clear();
	ui->attach8->clear();
	ui->attach9->clear();
	ui->attach10->clear();
}
