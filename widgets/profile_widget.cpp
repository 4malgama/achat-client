#include "profile_widget.h"
#include "ui_profile_widget.h"

ProfileWidget::ProfileWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ProfileWidget)
{
	ui->setupUi(this);
}

ProfileWidget::~ProfileWidget()
{
	delete ui;
}
