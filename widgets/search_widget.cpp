#include "search_widget.h"
#include "ui_search_widget.h"
#include "search_result_widget.h"
#include "../client.h"
#include "../network/account.h"


namespace searchwidget
{
	SearchWidget* w = nullptr;

	void close()
	{
		w = nullptr;
	}

	SearchWidget* getInstance()
	{
		if (w == nullptr)
		{
			w = new SearchWidget();
			QObject::connect(w, &SearchWidget::event_close, close);
		}
		return w;
	}
}

namespace client
{
	extern Client* window;
}

SearchWidget::SearchWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::SearchWidget)
{
	ui->setupUi(this);
	hide();
}

SearchWidget::~SearchWidget()
{
	delete ui;
}

void SearchWidget::addResult(SearchResultWidget *result)
{
	QVBoxLayout* layout = dynamic_cast<QVBoxLayout*>(ui->resultsContent->layout());
	if (layout == nullptr)
	{
		qInfo("Error find layout on Search Widget. Required 'QVBoxLayout'.");
		return;
	}
	layout->insertWidget(0, result);
}

void SearchWidget::addResults(const QList<SearchResultWidget*> results)
{
	for (SearchResultWidget* item : results)
		addResult(item);
}

void SearchWidget::clearResults()
{
	QLayout* layout = ui->resultsContent->layout();
	clearLayout(layout);
	layout->addItem(new QSpacerItem(0, 1, QSizePolicy::Minimum, QSizePolicy::Expanding));
}

void SearchWidget::on_btnSearch_clicked()
{
	onSearch(ui->leText->text());
}


void SearchWidget::on_leText_returnPressed()
{
	onSearch(ui->leText->text());
}

void SearchWidget::closeEvent(QCloseEvent *)
{
	emit event_close();
	delete this;
}

void SearchWidget::onSearch(const QString &text)
{
	clearResults();
	if (text.trimmed().isEmpty())
		return;
	client::window->acc->sendSearch(text.trimmed());
}

void SearchWidget::clearLayout(QLayout *l)
{
	if (l != nullptr)
	{
		while (QLayoutItem* child = l->takeAt(0))
		{
			if (QLayout* childLayout = child->layout())
				clearLayout(childLayout);
			else if (QWidget* w = child->widget())
				w->deleteLater();
			delete child;
		}
	}
}

