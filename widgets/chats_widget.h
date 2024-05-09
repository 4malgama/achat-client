#ifndef CHATS_WIDGET_H
#define CHATS_WIDGET_H

#include <QWidget>

class QFileInfo;
class ChatMessageWidget;
class ChatRowWidget;
struct InitChatData;
struct ChatData;
struct ChatMessage;

namespace Ui {
	class ChatsWidget;
}

class ChatsWidget : public QWidget
{
	Q_OBJECT

	QHash<quint64, ChatData> chats;
	ChatData* selectedChat = nullptr;
	ChatRowWidget* selectedRowChat = nullptr;
	QList<QString> attachments;

public:
	explicit ChatsWidget(QWidget *parent = nullptr);
	~ChatsWidget();

	void addChats(const QList<InitChatData>& chats);
	void addMessageToCurrentChat(ChatMessageWidget* message);
	void initMessages(quint64 chatId, const QList<ChatMessage>& messages);
	void addMessageToChat(quint64 chatId, ChatMessage* message, bool isMine);

signals:
	void event_close();

private:
	Ui::ChatsWidget *ui;
	void closeEvent(QCloseEvent *event);

	void onTextMessageChanged();
	void onSendClicked();
	void onAttachClicked();

	void clearLayout(QLayout* l);
	void addAttachment(const QFileInfo& info);
	void resetAttachments();
};

#endif // CHATS_WIDGET_H
