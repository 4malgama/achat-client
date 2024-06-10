#include "image_utils.h"
#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QPainterPath>


QImage ImageUtils::CropImageToCircle(const QImage &image)
{
	int size = qMin(image.width(), image.height());
	QImage squareImage = image.scaled(size, size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

	QImage circularImage(size, size, QImage::Format_ARGB32);
	circularImage.fill(Qt::transparent);

	QPainter painter(&circularImage);
	painter.setRenderHint(QPainter::Antialiasing);

	QPainterPath path;
	path.addEllipse(0, 0, size, size);
	painter.setClipPath(path);
	painter.drawImage(0, 0, squareImage);

	return circularImage;
}
