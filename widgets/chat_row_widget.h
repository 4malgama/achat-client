#ifndef CHATROWWIDGET_H
#define CHATROWWIDGET_H

#include "themed_widget.h"
#include <QPixmap>

class ChatRowWidget : public ThemedWidget
{
	Q_OBJECT

	QImage avatar;
	QString displayName;
	QString post;
	bool selected = false;
	bool hovered = false;
	bool pressed = false;

public:
	quint64 chatId;

	explicit ChatRowWidget(QWidget *parent = nullptr);
	ChatRowWidget(QWidget *parent, quint64 chatId, const QImage& avatar, const QString& displayName, const QString& post);

	void setAvatar(const QImage& avatar);
	void setDisplayName(const QString& displayName);
	void setPost(const QString& post);

	QImage getAvatar();
	QString getDisplayName();
	QString getPost();

	void setSelected(bool state);
	bool getSelected();

	void click();

protected:
	void onThemeChanged(const ThemeData& theme) override;

signals:
	void clicked();

private:
	void initialize();
	void paintEvent(QPaintEvent* event) override;
	void enterEvent(QEvent *event) override;
	void leaveEvent(QEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // CHATROWWIDGET_H
