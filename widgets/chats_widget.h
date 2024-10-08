#ifndef CHATS_WIDGET_H
#define CHATS_WIDGET_H

#include <QWidget>

class GPTService;
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

	const int MAX_FILE_SIZE;

	QHash<quint64, ChatData> chats;
	QHash<quint64, ChatData> bots;
	ChatData* selectedChat = nullptr;
	ChatRowWidget* selectedRowChat = nullptr;
	QList<QString> attachments;
	bool gptEnabled = false;
	bool selectedAI = false;

	GPTService* gpt;

public:
	explicit ChatsWidget(QWidget *parent = nullptr);
	~ChatsWidget();

	void addChats(const QList<InitChatData>& chats);
	void addMessageToCurrentChat(ChatMessageWidget* message);
	void initMessages(quint64 chatId, const QList<ChatMessage>& messages);
	void addMessageToChat(quint64 chatId, ChatMessage* message, bool isMine);
	void addMessageToBot(quint64 botId, ChatMessage* message, bool isMine);
	const ChatData* getChatData(quint64 chatId) const;
	void updateChatId(quint64 chatId, const QString &login);

	void clearCurrentChat();

	void openChat(quint64 id);
	void addChatGPT();

signals:
	void event_close();

private:
	Ui::ChatsWidget *ui;
	void closeEvent(QCloseEvent *event);
	void showEvent(QShowEvent*);

	void onTextMessageChanged();
	void onSendClicked();
	void onAttachClicked();

	void clearLayout(QLayout* l);
	void addAttachment(const QFileInfo& info);
	void resetAttachments();
};

#endif // CHATS_WIDGET_H
