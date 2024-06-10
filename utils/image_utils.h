#ifndef IMAGEUTILS_H
#define IMAGEUTILS_H

class QImage;

class ImageUtils
{
public:
	static QImage CropImageToCircle(const QImage& image);
};

#endif // IMAGEUTILS_H
