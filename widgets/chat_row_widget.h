#ifndef CHATROWWIDGET_H
#define CHATROWWIDGET_H

#include <QWidget>
#include <QPixmap>

class ChatRowWidget : public QWidget
{
	Q_OBJECT

	QImage avatar;
	QString displayName;
	QString post;
	bool selected = false;
	bool hovered = false;
	bool pressed = false;

	struct
	{
		struct
		{
			QColor common = QColor(36, 64, 89);
			QColor selected = QColor(59, 104, 145);
			QColor hovered = QColor(60, 92, 121);
			QColor pressed = QColor(44, 59, 72);
		} bg;
	} colors;

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
