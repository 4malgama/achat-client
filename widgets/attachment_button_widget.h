#ifndef ATTACHMENTBUTTONWIDGET_H
#define ATTACHMENTBUTTONWIDGET_H

#include <QPushButton>

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
	double fileSize;	//Kb
	QPixmap pixmap;
	Q_PROPERTY(QString fileName READ getFileName WRITE setFileName NOTIFY fileNameChanged FINAL)
	Q_PROPERTY(quint32 fileSize READ getFileSize WRITE setFileSize NOTIFY fileSizeChanged FINAL)
	Q_PROPERTY(QPixmap pixmap READ getPixmap WRITE setPixmap NOTIFY pixmapChanged FINAL)

	bool hovered = false;

	void paintEvent(QPaintEvent*) override;
	void enterEvent(QEvent*) override;
	void leaveEvent(QEvent*) override;
};

#endif // ATTACHMENTBUTTONWIDGET_H
