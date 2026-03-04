#include "image_utils.h"
#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QPainterPath>


namespace crypto
{
	QString MD5(const QString& data);
}

QImage ImageUtils::CropImageToCircle(const QImage &image, int size)
{
	int _size = (size != -1 ? size : qMin(image.width(), image.height()));
	QImage squareImage = image.scaled(_size, _size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

	QImage circularImage(_size, _size, QImage::Format_ARGB32);
	circularImage.fill(Qt::transparent);

	QPainter painter(&circularImage);
	painter.setRenderHint(QPainter::Antialiasing);

	QPainterPath path;
	path.addEllipse(0, 0, _size, _size);
	painter.setClipPath(path);
	painter.drawImage(0, 0, squareImage);

	return circularImage;
}

QImage ImageUtils::GetImageFromName(const QString &name)
{
	int size = 512;
	QString hash = crypto::MD5(name);
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
