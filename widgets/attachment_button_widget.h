#ifndef ATTACHMENTBUTTONWIDGET_H
#define ATTACHMENTBUTTONWIDGET_H

#include <QPushButton>
#include <QColor>

class ThemeData;

class AttachmentButtonWidget : public QPushButton
{
	Q_OBJECT

public:
	AttachmentButtonWidget(QWidget* parent = nullptr);

	QString getFileName() const;
	void setFileName(const QString &newFileName);

	double getFileSize() const;
	void setFileSize(double newFileSize);

	QPixmap getPixmap() const;
	void setPixmap(const QPixmap &newPixmap);

signals:
	void fileNameChanged();

	void fileSizeChanged();

	void pixmapChanged();

private:
	QString fileName;
	double fileSize = 0.0;	//Kb
	QPixmap pixmap;
	Q_PROPERTY(QString fileName READ getFileName WRITE setFileName NOTIFY fileNameChanged FINAL)
	Q_PROPERTY(double fileSize READ getFileSize WRITE setFileSize NOTIFY fileSizeChanged FINAL)
	Q_PROPERTY(QPixmap pixmap READ getPixmap WRITE setPixmap NOTIFY pixmapChanged FINAL)

	bool hovered = false;

	QColor textColor;
	QColor textSecondary;
	QColor backgroundColor;
	QColor hoverColor;
	QColor borderColor;
	int cornerRadius = 0;
	int borderWidth = 1;
	int spacing = 8;

protected:
	void paintEvent(QPaintEvent*) override;
	void enterEvent(QEvent*) override;
	void leaveEvent(QEvent*) override;

private:
	void applyTheme(const ThemeData& theme);
};

#endif // ATTACHMENTBUTTONWIDGET_H
