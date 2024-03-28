#ifndef ADVERTWIDGET_H
#define ADVERTWIDGET_H

#include <QWidget>

class AdvertWidget : public QWidget
{
	Q_OBJECT

	struct
	{
		QColor a;
		QColor b;
	} background;

	QString title;
	QString description;
	QString link;
	QString linkText;

	QImage image;

public:
	explicit AdvertWidget(QWidget *parent = nullptr);
	AdvertWidget(QColor a, QColor b, QWidget *parent = nullptr);

	void setTitle(const QString& title);
	void setDescription(const QString& description);
	void setLink(const QString& link);
	void setLinkText(const QString& linkText);
	void setImage(const QImage& image);

signals:

private:
	void paintEvent(QPaintEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
};

#endif // ADVERTWIDGET_H
