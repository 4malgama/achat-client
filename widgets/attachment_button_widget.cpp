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
	update();
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
	update();
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
	update();
}

void AttachmentButtonWidget::paintEvent(QPaintEvent *e)
{
	Q_UNUSED(e)

	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

	QRectF backgroundRect = rect();
	backgroundRect.adjust(borderWidth / 2.0, borderWidth / 2.0, -borderWidth / 2.0, -borderWidth / 2.0);
	painter.setPen(QPen(borderColor, borderWidth));
	painter.setBrush(hovered ? hoverColor : backgroundColor);
	painter.drawRoundedRect(backgroundRect, cornerRadius, cornerRadius);

	const int iconExtent = qMax(16, height() - spacing);
	const QRect iconRect(spacing / 2, (height() - iconExtent) / 2, iconExtent, iconExtent);
	painter.drawPixmap(iconRect, pixmap);

	painter.setFont(font());
	const int textLeft = iconRect.right() + spacing;
	const int textWidth = qMax(0, width() - textLeft - spacing);
	const int halfHeight = height() / 2;
	const QFontMetrics metrics(font());

	painter.setPen(textColor);
	painter.drawText(
		QRect(textLeft, 0, textWidth, halfHeight),
		Qt::AlignLeft | Qt::AlignBottom,
		metrics.elidedText(fileName, Qt::ElideMiddle, textWidth)
	);

	painter.setPen(textSecondary);
	painter.drawText(
		QRect(textLeft, halfHeight, textWidth, height() - halfHeight),
		Qt::AlignLeft | Qt::AlignTop,
		QString::number(fileSize, 'f', 2) + tr(" Kb")
	);
}

void AttachmentButtonWidget::enterEvent(QEvent *)
{
	hovered = true;
	update();
}

void AttachmentButtonWidget::leaveEvent(QEvent *)
{
	hovered = false;
	update();
}

void AttachmentButtonWidget::applyTheme(const ThemeData &theme)
{
	setFont(theme.fonts.button);

	textColor = theme.colors.textPrimary;

	textSecondary = theme.colors.textSecondary;

	backgroundColor = QColor(0, 0, 0, 100);
	hoverColor = theme.colors.surfaceHover;
	borderColor = theme.colors.border;
	cornerRadius = theme.radii.medium;
	borderWidth = theme.metrics.borderWidth;
	spacing = theme.metrics.spacingSm;

	update();
}
