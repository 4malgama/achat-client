#ifndef SEARCHRESULTWIDGET_H
#define SEARCHRESULTWIDGET_H

#include <QWidget>


class SearchResultWidget : public QWidget
{
	Q_OBJECT
public:
	explicit SearchResultWidget(QWidget *parent = nullptr);
	SearchResultWidget(QWidget *parent, const QPixmap& pixmap, const QString& login, const QString& displayName);

	QPixmap getPixmap() const;
	void setPixmap(const QPixmap &newPixmap);

	QString getLogin() const;
	void setLogin(const QString &newLogin);

	QString getDisplayName() const;
	void setDisplayName(const QString &newDisplayName);

signals:
	void pixmapChanged();
	void loginChanged();
	void displayNameChanged();
	void clicked();

private slots:
	void onSomeChanged();

protected:
	void paintEvent(QPaintEvent*) override;
	void enterEvent(QEvent*) override;
	void leaveEvent(QEvent*) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;

private:
	struct
	{
		struct
		{
			QColor common = QColor(36, 64, 89);
			QColor hovered = QColor(60, 92, 121);
			QColor pressed = QColor(44, 59, 72);
		} bg;
	} colors;
	QFont font;

	bool hovered;
	bool pressed;

	QPixmap pixmap;
	QString login;
	QString displayName;
	Q_PROPERTY(QPixmap pixmap READ getPixmap WRITE setPixmap NOTIFY pixmapChanged FINAL)
	Q_PROPERTY(QString login READ getLogin WRITE setLogin NOTIFY loginChanged FINAL)
	Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged FINAL)
};

#endif // SEARCHRESULTWIDGET_H
