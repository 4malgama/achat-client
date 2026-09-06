#ifndef CONSOLE_WIDGET_H
#define CONSOLE_WIDGET_H

#include "themed_widget.h"

namespace Ui {
	class ConsoleWidget;
}

class ConsoleWidget : public ThemedWidget
{
	Q_OBJECT

public:
	explicit ConsoleWidget(QWidget *parent = nullptr);
	~ConsoleWidget();

	void writeLine(const QString& text);
	void writeBufferedLine(const QString& text);

signals:
	void event_close();

protected:
	void onThemeChanged(const ThemeData& theme) override;

private slots:
	void on_lineEdit_returnPressed();

private:
	Ui::ConsoleWidget *ui;
	void closeEvent(QCloseEvent*) override;
	void showEvent(QShowEvent*) override;

	void processCommand(const QString& command, const QStringList& args);
	void printHelp();
};

#endif // CONSOLE_WIDGET_H
