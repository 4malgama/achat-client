#include "attachment_button_widget.h"
#include "../theme_manager/theme_binding.h"
#include <QPainter>


AttachmentButtonWidget::AttachmentButtonWidget(QWidget *parent)
	: QPushButton(parent)
{
	setCursor(Qt::PointingHandCursor);

	theme::bind(this, [this] (const ThemeData& theme) { applyTheme(theme); });
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

void AttachmentButtonWidget::paintEvent(QPaintEvent *e)
{
	Q_UNUSED(e)

	QPainter painter(this);

	//background
	painter.setPen(Qt::NoPen);
	painter.setBrush(hovered ? hoverColor : Qt::transparent);
	painter.drawRoundedRect(rect(), 5, 5);

	//pixmap
	painter.drawPixmap(0, 0, 30, 30, pixmap);

	//text
	painter.setFont(font());

	painter.setPen(textColor);
	painter.drawText(40, 13, fileName);

	painter.setPen(textSecondary);
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

void AttachmentButtonWidget::applyTheme(const ThemeData &theme)
{
	setFont(theme.fonts.button);

	textColor = theme.colors.textPrimary;

	textSecondary = theme.colors.textSecondary;

	hoverColor = theme.colors.textPrimary;
	hoverColor.setAlphaF(0.35f);

	update();
}
