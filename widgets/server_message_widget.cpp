#include "server_message_widget.h"
#include "ui_server_message_widget.h"

#include <QPainter>

ServerMessageWidget::ServerMessageWidget(QWidget *parent, const QString& text) :
	QDialog(parent),
	ui(new Ui::ServerMessageWidget),
	text(text)
{
	ui->setupUi(this);
	ui->label->setText(text);

	connect(ui->pushButton, &QAbstractButton::clicked, this, &ServerMessageWidget::close);

	setWindowFlags(Qt::WindowType::FramelessWindowHint | Qt::WindowType::WindowStaysOnTopHint);
	setAttribute(Qt::WidgetAttribute::WA_TranslucentBackground);

	if (parent != nullptr)
		move(parent->width() / 2 - width() / 2, parent->height() / 2 - height() / 2);

	show();
}

ServerMessageWidget::~ServerMessageWidget()
{
	delete ui;
}

/*
*  This function unsafe to use.
*  Because it doesn't check parent widget.
*  
*  More info: https://doc.qt.io/qt-5/qdialog.html#exec
*/

// int ServerMessageWidget::exec()
// {
// 	return QDialog::exec();
// }

void ServerMessageWidget::open()
{
	QDialog::open();
}

// int ServerMessageWidget::exec(QWidget* parent, const QString& text)
// {
// 	if (parent == nullptr)
// 		return -1;

// 	ServerMessageWidget* widget = new ServerMessageWidget(parent, text);
// 	return widget->exec();
// }

void ServerMessageWidget::open(QWidget* parent, const QString& text)
{
	if (parent == nullptr)
		return;

	ServerMessageWidget* widget = new ServerMessageWidget(parent, text);
	widget->open();
}

void ServerMessageWidget::paintEvent(QPaintEvent *event)
{
	QColor backgroundColor(130, 130, 130);

	QPainter painter(this);
	painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing | QPainter::Antialiasing);

	QBrush brush(backgroundColor);
	QPen pen(Qt::white);
	pen.setWidth(2);
	painter.setPen(pen);
	painter.setBrush(brush);
	painter.drawRoundedRect(rect(), 10, 10);
}
