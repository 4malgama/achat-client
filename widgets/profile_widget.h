#ifndef PROFILE_WIDGET_H
#define PROFILE_WIDGET_H

#include <QWidget>

namespace Ui {
	class ProfileWidget;
}

class ProfileWidget : public QWidget
{
	Q_OBJECT

public:
	explicit ProfileWidget(QWidget *parent = nullptr);
	~ProfileWidget();

private:
	Ui::ProfileWidget *ui;
};

#endif // PROFILE_WIDGET_H
