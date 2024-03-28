#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Client; }
QT_END_NAMESPACE

class Account;
class ProfileWidget;
class AuthorizationWidget;

class Client : public QMainWindow
{
	Q_OBJECT

	QWidget* currentPage = nullptr;

	ProfileWidget* pw = nullptr;
	AuthorizationWidget* authWidget = nullptr;

	QPoint lastDragPos;
	bool dragging = false;
	bool isMaximized = false;

public:
	Client(QWidget *parent = nullptr);
	~Client();

	Account* acc;

	void openMyProfilePage();
	void authWindow();
	void closePages();

	void enableSideButtons();
	void disableSideButtons();

	void addAdvertPage(QWidget* page);

signals:
	void event_close();

private slots:
	void on_btnProfile_clicked();
	void on_btnClose_clicked();
	void on_btnSize_clicked();
	void on_btnHide_clicked();

private:
	Ui::Client *ui;
	void closeEvent(QCloseEvent*);
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
};
#endif // CLIENT_H
