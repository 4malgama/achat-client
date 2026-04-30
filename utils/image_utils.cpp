#include "image_utils.h"
#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QPainterPath>


namespace crypto
{
	QString md5(const QString& data);
}

QImage ImageUtils::makeCircularAvatar(const QImage& image, int size)
{
	if (image.isNull())
		return {};

	const int side = qMin(image.width(), image.height());
	const int targetSize = size > 0 ? size : side;

	const QRect cropRect(
		(image.width() - side) / 2,
		(image.height() - side) / 2,
		side,
		side
	);

	QImage squareImage = image.copy(cropRect).scaled(
		targetSize,
		targetSize,
		Qt::KeepAspectRatioByExpanding,
		Qt::SmoothTransformation
	);

	QImage circularImage(targetSize, targetSize, QImage::Format_ARGB32_Premultiplied);
	circularImage.fill(Qt::transparent);

	QPainter painter(&circularImage);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

	QPainterPath path;
	path.addEllipse(QRectF(0, 0, targetSize, targetSize));

	painter.setClipPath(path);
	painter.drawImage(0, 0, squareImage);

	return circularImage;
}

QImage ImageUtils::makeImageFromName(const QString &name)
{
	int size = 512;
	QString hash = crypto::md5(name);
	QRgb rgb_a = hash.midRef(0, 6).toUInt(nullptr, 16);
	QRgb rgb_b = hash.midRef(6, 6).toUInt(nullptr, 16);

	QImage image(size, size, QImage::Format_ARGB32);

	QPainter painter(&image);
	painter.setRenderHint(QPainter::TextAntialiasing);

	QLinearGradient bgGradient(0, 0, 0, size);
	bgGradient.setColorAt(0, QColor::fromRgb(rgb_a));
	bgGradient.setColorAt(1, QColor::fromRgb(rgb_b));

	painter.fillRect(image.rect(), bgGradient);

	painter.setPen(Qt::white);
	painter.setFont(QFont("Calibri", 172, QFont::Bold));

	QString text;
	if (name.isEmpty())
	{
		text = "X";
	}
	else
	{
		text = name.at(0);
	}

	QRect textRect = painter.boundingRect(image.rect(), Qt::AlignHCenter | Qt::AlignVCenter | Qt::TextWordWrap, text.toUpper());
	painter.drawText(textRect, Qt::AlignHCenter | Qt::AlignVCenter | Qt::TextWordWrap, text.toUpper());

	return image;
}
