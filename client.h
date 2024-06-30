#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Client; }
QT_END_NAMESPACE

class Account;
class ProfileWidget;
class AuthorizationWidget;
class ChatsWidget;
class SearchWidget;
struct InitChatData;
struct ChatMessage;

class Client : public QMainWindow
{
	Q_OBJECT

	QWidget* currentPage = nullptr;

	ProfileWidget* pw = nullptr;
	AuthorizationWidget* authWidget = nullptr;
	ChatsWidget* cw = nullptr;
	SearchWidget* sw = nullptr;

	QPoint lastDragPos;
	bool dragging = false;
	bool isMaximized = false;


public:
	Client(QWidget *parent = nullptr);
	~Client();

	Account* acc;

	void openMyProfilePage();
	void openChatsPage();
	void openSearchPage();
	void authWindow();
	void closeAuthWindow();
	void closePages();
	void showMessage(const QString& text, quint8 icon = 0);

	void addMessageToChat(quint64 chatId, ChatMessage* message);
	void initChats(const QList<InitChatData>& chats);
	void initMessages(quint64 chatId, const QList<ChatMessage>& messages);

	void enableSideButtons();
	void disableSideButtons();

	void addAdvertPage(QWidget* page);
	void setProfileData(const QHash<QString, QVariant>& profileInfo);

	void openChat(quint64 id);
	void createChatGPT();

signals:
	void event_close();
	void event_resize(QSize size);

private slots:
	void on_btnProfile_clicked();
	void on_btnClose_clicked();
	void on_btnSize_clicked();
	void on_btnHide_clicked();

	void on_btnChats_clicked();

	void on_btnConsole_clicked();

	void on_btnSearch_clicked();

private:
	Ui::Client *ui;
	void closeEvent(QCloseEvent*);
	void resizeEvent(QResizeEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
};
#endif // CLIENT_H
