#include "attachment_button_widget.h"
#include <QPainter>

#include <QDebug>

AttachmentButtonWidget::AttachmentButtonWidget(QWidget *parent)
	: QPushButton(parent)
{
	setText(QString());
	setCursor(Qt::PointingHandCursor);
}

QString AttachmentButtonWidget::getFileName() const
{
	return fileName;
}

void AttachmentButtonWidget::setFileName(const QString &newFileName)
{
	if (fileName == newFileName)
		return;
	fileName = newFileName;
	emit fileNameChanged();
}

double AttachmentButtonWidget::getFileSize() const
{
	return fileSize;
}

void AttachmentButtonWidget::setFileSize(double newFileSize)
{
	if (fileSize == newFileSize)
		return;
	fileSize = newFileSize;
	emit fileSizeChanged();
}

QPixmap AttachmentButtonWidget::getPixmap() const
{
	return pixmap;
}

void AttachmentButtonWidget::setPixmap(const QPixmap &newPixmap)
{
	if (pixmap == newPixmap)
		return;
	pixmap = newPixmap;
	emit pixmapChanged();
}

void AttachmentButtonWidget::paintEvent(QPaintEvent *)
{
	QPainter painter(this);

	//background
	painter.setPen(Qt::NoPen);
	painter.setBrush(hovered ? QColor(255, 255, 255, 120) : Qt::transparent);
	painter.drawRoundedRect(rect(), 5, 5);

	//pixmap
	painter.drawPixmap(0, 0, 30, 30, pixmap);

	//text
	QFont font("Segoe UI", 9);

	painter.setFont(font);

	painter.setPen(Qt::white);
	painter.drawText(40, 13, fileName);

	painter.setPen(Qt::gray);
	painter.drawText(40, 27, QString::number(fileSize, 'f', 2) + tr(" Kb"));
}

void AttachmentButtonWidget::enterEvent(QEvent *)
{
	hovered = true;
}

void AttachmentButtonWidget::leaveEvent(QEvent *)
{
	hovered = false;
}
