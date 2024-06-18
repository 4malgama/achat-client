#ifndef IMAGEUTILS_H
#define IMAGEUTILS_H

class QImage;
class QString;

class ImageUtils
{
public:
	static QImage CropImageToCircle(const QImage& image, int size = -1);
	static QImage GetImageFromName(const QString& name);
};

#endif // IMAGEUTILS_H
