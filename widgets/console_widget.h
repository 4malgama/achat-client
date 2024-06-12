#ifndef CONSOLE_WIDGET_H
#define CONSOLE_WIDGET_H

#include <QWidget>

namespace Ui {
	class ConsoleWidget;
}

class ConsoleWidget : public QWidget
{
	Q_OBJECT

public:
	explicit ConsoleWidget(QWidget *parent = nullptr);
	~ConsoleWidget();

	void writeLine(const QString& text);

signals:
	void event_close();

private slots:
	void on_lineEdit_returnPressed();

private:
	Ui::ConsoleWidget *ui;
	void closeEvent(QCloseEvent*) override;

	void processCommand(const QString& command);
	void printHelp();
};

#endif // CONSOLE_WIDGET_H
