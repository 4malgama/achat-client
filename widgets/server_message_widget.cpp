#include "server_message_widget.h"
#include "ui_server_message_widget.h"
#include "../theme_manager/theme_binding.h"

#include <QPainter>

ServerMessageWidget::ServerMessageWidget(QWidget *parent, const QString& text) :
	QDialog(parent),
	ui(new Ui::ServerMessageWidget),
	text(text)
{
	ui->setupUi(this);
	ui->label->setText(text);
	ui->label->setWordWrap(true);

	connect(ui->pushButton, &QAbstractButton::clicked, this, &ServerMessageWidget::close);

	setWindowFlags(Qt::WindowType::FramelessWindowHint | Qt::WindowType::WindowStaysOnTopHint);
	setAttribute(Qt::WidgetAttribute::WA_TranslucentBackground);
	setAttribute(Qt::WidgetAttribute::WA_DeleteOnClose);

	theme::bind(this, [this] (const ThemeData& theme) {
		setFont(theme.fonts.base);
		ui->label->setFont(theme.fonts.base);
		ui->pushButton->setFont(theme.fonts.button);
		ui->verticalLayout->setContentsMargins(
			theme.metrics.spacingLg,
			theme.metrics.spacingLg,
			theme.metrics.spacingLg,
			theme.metrics.spacingLg
		);
		ui->verticalLayout->setSpacing(theme.metrics.spacingSm);
	});

	if (parent != nullptr)
		move(parent->width() / 2 - width() / 2, parent->height() / 2 - height() / 2);

	show();
}

ServerMessageWidget::~ServerMessageWidget()
{
	delete ui;
}

void ServerMessageWidget::open()
{
	QDialog::open();
}

void ServerMessageWidget::open(QWidget* parent, const QString& text)
{
	if (parent == nullptr)
		return;

	ServerMessageWidget* widget = new ServerMessageWidget(parent, text);
	widget->open();
}

void ServerMessageWidget::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event)

	const ThemeData& theme = theme::current();

	QPainter painter(this);
	painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing | QPainter::Antialiasing);

	QRectF backgroundRect = rect();
	backgroundRect.adjust(
		theme.metrics.borderWidth / 2.0,
		theme.metrics.borderWidth / 2.0,
		-theme.metrics.borderWidth / 2.0,
		-theme.metrics.borderWidth / 2.0
	);
	painter.setPen(QPen(theme.colors.borderStrong, theme.metrics.borderWidth));
	painter.setBrush(theme.colors.panelBackground);
	painter.drawRoundedRect(backgroundRect, theme.radii.large, theme.radii.large);
}
