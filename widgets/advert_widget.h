#ifndef ADVERTWIDGET_H
#define ADVERTWIDGET_H

#include "themed_widget.h"
#include <QImage>
#include <QPropertyAnimation>

class AdvertWidget : public ThemedWidget
{
	Q_OBJECT

	qreal m_offset = 0.0;
	QPropertyAnimation* anim;

	QString title;
	QString description;
	QString link;
	QString linkText;

	QImage image;

public:
	explicit AdvertWidget(QWidget *parent = nullptr);

	void setTitle(const QString& title);
	void setDescription(const QString& description);
	void setLink(const QString& link);
	void setLinkText(const QString& linkText);
	void setImage(const QImage& image);

	qreal offset() const;
	void setOffset(qreal newOffset);

signals:

	void offsetChanged();

private:
	void paintEvent(QPaintEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void onThemeChanged(const ThemeData& theme) override;

	Q_PROPERTY(qreal offset READ offset WRITE setOffset NOTIFY offsetChanged FINAL)
};

#endif // ADVERTWIDGET_H
