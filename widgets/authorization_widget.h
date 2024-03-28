#ifndef AUTHORIZATION_WIDGET_H
#define AUTHORIZATION_WIDGET_H

#include <QWidget>

namespace Ui {
	class AuthorizationWidget;
}

class AuthorizationWidget : public QWidget
{
	Q_OBJECT

	bool registerState = false;

public:
	explicit AuthorizationWidget(QWidget *parent = nullptr, bool registerState = false);
	~AuthorizationWidget();

	void updateForm();

signals:
	void event_close();

private slots:
	void onInputChanged();
	void onLoginClicked();
	void onSwitchFormClicked();

private:
	Ui::AuthorizationWidget *ui;
	void closeEvent(QCloseEvent *event);
	void paintEvent(QPaintEvent *event);
};

#endif // AUTHORIZATION_WIDGET_H
