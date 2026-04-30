#ifndef IMAGEUTILS_H
#define IMAGEUTILS_H

class QImage;
class QString;

class ImageUtils
{
public:
	static QImage makeCircularAvatar(const QImage& image, int size = -1);
	static QImage makeImageFromName(const QString& name);
};

#endif // IMAGEUTILS_H
