#include "authorization_widget.h"
#include "ui_authorization_widget.h"
#include "../client.h"
#include "../network/account.h"
#include "server_message_widget.h"

#include <QPainter>
#include <QCryptographicHash>

namespace client { extern Client* window; }
namespace auth
{
	bool remember = false;
	QString login;
	QString password;
}

QByteArray HashPassword(const QString& password)
{
	return QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Algorithm::Md5).toHex();
}

AuthorizationWidget::AuthorizationWidget(QWidget *parent, bool registerState) :
	QWidget(parent),
	ui(new Ui::AuthorizationWidget),
	registerState(registerState)
{
	ui->setupUi(this);
	setWindowFlags(Qt::WindowType::FramelessWindowHint | Qt::WindowType::WindowStaysOnTopHint);
	setAttribute(Qt::WidgetAttribute::WA_TranslucentBackground);
	setWindowTitle("Authorization");

	connect(ui->le_login, &QLineEdit::textChanged, this, &AuthorizationWidget::onInputChanged);
	connect(ui->le_password, &QLineEdit::textChanged, this, &AuthorizationWidget::onInputChanged);
	connect(ui->le_confirm, &QLineEdit::textChanged, this, &AuthorizationWidget::onInputChanged);
	connect(ui->btnLogin, &QAbstractButton::clicked, this, &AuthorizationWidget::onLoginClicked);
	connect(ui->btnSwitchForm, &QAbstractButton::clicked, this, &AuthorizationWidget::onSwitchFormClicked);

	updateForm();
}

AuthorizationWidget::~AuthorizationWidget()
{
	delete ui;
}

void AuthorizationWidget::closeEvent(QCloseEvent *event)
{
	emit event_close();
	delete this;
}

void AuthorizationWidget::paintEvent(QPaintEvent *event)
{
	QColor backgroundColor(18, 70, 108);

	QPainter painter(this);
	painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing | QPainter::Antialiasing);

	QBrush brush(backgroundColor);
	QPen pen(Qt::white);
	pen.setWidth(2);
	painter.setPen(pen);
	painter.setBrush(brush);
	painter.drawRoundedRect(rect(), 10, 10);
}

void AuthorizationWidget::updateForm()
{
	if (registerState)
	{
		ui->btnSwitchForm->setText(tr("Login"));
		ui->btnLogin->setText(tr("Register"));
		ui->title->setText(tr("Create account"));
		ui->label->show();
		ui->le_confirm->show();
	}
	else
	{
		ui->btnSwitchForm->setText(tr("Register"));
		ui->btnLogin->setText(tr("Login"));
		ui->title->setText(tr("Login"));
		ui->label->hide();
		ui->le_confirm->hide();
	}
	onInputChanged();
}

void AuthorizationWidget::onInputChanged()
{
	bool isInputValid = !ui->le_login->text().isEmpty() && !ui->le_password->text().isEmpty();

	if (registerState)
		ui->btnLogin->setEnabled(isInputValid && !ui->le_confirm->text().isEmpty());
	else
		ui->btnLogin->setEnabled(isInputValid);
}

void AuthorizationWidget::onLoginClicked()
{
	auth::login = ui->le_login->text();
	auth::password = ui->le_password->text();
	auth::remember = ui->cb_remember->isChecked();

	if (registerState)
	{
		QString confirm = ui->le_confirm->text();

		if (auth::password == confirm)
		{
			client::window->acc->registration(auth::login, auth::password);
		}
		else
		{
			client::window->showMessage(tr("Passwords do not match"), 3);
		}
	}
	else
	{
		client::window->acc->login(auth::login, HashPassword(auth::password));
	}

	ui->le_password->clear();
	ui->cb_remember->setChecked(false);
	ui->le_confirm->clear();
}

void AuthorizationWidget::onSwitchFormClicked()
{
	registerState = !registerState;
	updateForm();
}