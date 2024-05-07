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
struct InitChatData;

class Client : public QMainWindow
{
	Q_OBJECT

	QWidget* currentPage = nullptr;

	ProfileWidget* pw = nullptr;
	AuthorizationWidget* authWidget = nullptr;
	ChatsWidget* cw = nullptr;

	QPoint lastDragPos;
	bool dragging = false;
	bool isMaximized = false;


public:
	Client(QWidget *parent = nullptr);
	~Client();

	Account* acc;

	void openMyProfilePage();
	void openChatsPage();
	void authWindow();
	void closeAuthWindow();
	void closePages();
	void showMessage(const QString& text, quint8 icon = 0);

	void addMessageToChat(int chatId, const QString& text, quint64 timestamp, bool isMine, bool showAvatar = false);
	void initChats(const QList<InitChatData>& chats);

	void enableSideButtons();
	void disableSideButtons();

	void addAdvertPage(QWidget* page);
	void setProfileData(const QHash<QString, QVariant>& profileInfo);

signals:
	void event_close();
	void event_resize(QSize size);

private slots:
	void on_btnProfile_clicked();
	void on_btnClose_clicked();
	void on_btnSize_clicked();
	void on_btnHide_clicked();

	void on_btnChats_clicked();

private:
	Ui::Client *ui;
	void closeEvent(QCloseEvent*);
	void resizeEvent(QResizeEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
};
#endif // CLIENT_H
