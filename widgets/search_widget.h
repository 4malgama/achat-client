#ifndef SEARCH_WIDGET_H
#define SEARCH_WIDGET_H

#include "themed_widget.h"

namespace Ui {
	class SearchWidget;
}

class SearchResultWidget;

class SearchWidget : public ThemedWidget
{
	Q_OBJECT

public:
	explicit SearchWidget(QWidget *parent = nullptr);
	~SearchWidget();

	void addResult(SearchResultWidget* result);
	void addResults(const QList<SearchResultWidget*> results);
	void clearResults();

private slots:
	void on_btnSearch_clicked();

	void on_leText_returnPressed();

signals:
	void event_close();

protected:
	void onThemeChanged(const ThemeData& theme) override;

private:
	Ui::SearchWidget *ui;
	void closeEvent(QCloseEvent*) override;
	void onSearch(const QString& text);
	void clearLayout(QLayout* l);
};

#endif // SEARCH_WIDGET_H
