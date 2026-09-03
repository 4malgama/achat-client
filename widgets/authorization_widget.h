#ifndef AUTHORIZATION_WIDGET_H
#define AUTHORIZATION_WIDGET_H

#include "themed_widget.h"

namespace Ui {
	class AuthorizationWidget;
}

class AuthorizationWidget : public ThemedWidget
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

protected:
	void onThemeChanged(const ThemeData& theme) override;

private:
	Ui::AuthorizationWidget *ui;
	void closeEvent(QCloseEvent *event) override;
	void paintEvent(QPaintEvent *event) override;
};

#endif // AUTHORIZATION_WIDGET_H
