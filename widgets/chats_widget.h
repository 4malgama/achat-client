#ifndef CHATS_WIDGET_H
#define CHATS_WIDGET_H

#include <QWidget>

class ChatMessageWidget;
class ChatRowWidget;
struct InitChatData;
struct ChatData;

namespace Ui {
	class ChatsWidget;
}

class ChatsWidget : public QWidget
{
	Q_OBJECT

	QHash<quint64, ChatData> chats;

public:
	explicit ChatsWidget(QWidget *parent = nullptr);
	~ChatsWidget();

	void addChats(const QList<InitChatData>& chats);
	void addMessageToCurrentChat(ChatMessageWidget* message);

signals:
	void event_close();

private:
	Ui::ChatsWidget *ui;
	void closeEvent(QCloseEvent *event);

	void onTextMessageChanged();
};

#endif // CHATS_WIDGET_H
