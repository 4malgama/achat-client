#ifndef DROPDOWNMENU_H
#define DROPDOWNMENU_H

#include <QMenu>

class QPropertyAnimation;

class DropdownMenu : public QMenu
{
	Q_OBJECT

public:
	DropdownMenu(QWidget* parent = nullptr);

	bool toRight;

	static QAction* makeAction(const QString& text, QMenu* parentMenu, void (* slot) ());

protected:
	void showEvent(QShowEvent*) override;
	void paintEvent(QPaintEvent*) override;

	QPropertyAnimation* animation;
};

#endif // DROPDOWNMENU_H
