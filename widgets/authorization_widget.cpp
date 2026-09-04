#include "authorization_widget.h"
#include "ui_authorization_widget.h"
#include "../client.h"
#include "../network/account.h"

#include <QPainter>


namespace client { extern Client* window; }
namespace auth
{
	bool remember = false;
	QString login;
	QString password;
}

AuthorizationWidget::AuthorizationWidget(QWidget *parent, bool registerState) :
	ThemedWidget(parent),
	registerState(registerState),
	ui(new Ui::AuthorizationWidget)
{
	ui->setupUi(this);

	setWindowFlags(Qt::WindowType::FramelessWindowHint | Qt::WindowType::WindowStaysOnTopHint);
	setAttribute(Qt::WidgetAttribute::WA_TranslucentBackground);
	setWindowTitle(tr("Authorization"));

	connect(ui->le_login, &QLineEdit::textChanged, this, &AuthorizationWidget::onInputChanged);
	connect(ui->le_password, &QLineEdit::textChanged, this, &AuthorizationWidget::onInputChanged);
	connect(ui->le_confirm, &QLineEdit::textChanged, this, &AuthorizationWidget::onInputChanged);
	connect(ui->btnLogin, &QAbstractButton::clicked, this, &AuthorizationWidget::onLoginClicked);
	connect(ui->btnSwitchForm, &QAbstractButton::clicked, this, &AuthorizationWidget::onSwitchFormClicked);

	updateForm();
	onThemeChanged(theme());
}

AuthorizationWidget::~AuthorizationWidget()
{
	delete ui;
}

void AuthorizationWidget::closeEvent(QCloseEvent *event)
{
	Q_UNUSED(event)

	emit event_close();
	delete this;
}

void AuthorizationWidget::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);

	const ThemeData& t = theme();

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

	QRectF r = rect();
	r.adjust(
		t.metrics.borderWidth / 2.0,
		t.metrics.borderWidth / 2.0,
		-t.metrics.borderWidth / 2.0,
		-t.metrics.borderWidth / 2.0
	);

	painter.setPen(QPen(t.colors.borderStrong, t.metrics.borderWidth));
	painter.setBrush(t.colors.panelBackground);

	painter.drawRoundedRect(r, t.radii.large, t.radii.large);
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
	layout()->activate();
	adjustSize();
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
		client::window->acc->login(auth::login, auth::password);
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

void AuthorizationWidget::onThemeChanged(const ThemeData &theme)
{
	setFont(theme.fonts.base);

	if (ui->title)
		ui->title->setFont(theme.fonts.title);

	ui->label->setFont(theme.fonts.base);
	ui->label_2->setFont(theme.fonts.base);
	ui->label_3->setFont(theme.fonts.base);
	ui->label_4->setFont(theme.fonts.base);

	ui->le_login->setFont(theme.fonts.base);
	ui->le_login->setFixedHeight(theme.metrics.controlHeight);

	ui->le_password->setFont(theme.fonts.base);
	ui->le_password->setFixedHeight(theme.metrics.controlHeight);

	ui->le_confirm->setFont(theme.fonts.base);
	ui->le_confirm->setFixedHeight(theme.metrics.controlHeight);

	layout()->setSpacing(theme.metrics.spacingSm);
	ui->formLayout->setSpacing(theme.metrics.spacingSm);

	layout()->activate();
	adjustSize();
	updateGeometry();
	update();
}
