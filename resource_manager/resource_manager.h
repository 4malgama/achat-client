#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QObject>
#include <QMap>
#include <QImage>


class ResourceManager : public QObject
{
	Q_OBJECT

	bool loaded;
	QString m_Path;
	QMap<QString, QImage> images;

private:
	explicit ResourceManager(QObject *parent = nullptr);

public:
	static ResourceManager& instance();
	void load();
	bool isLoaded();

	QImage getImage(const QString& name);

	QImage getAvatar();
	void setAvatar(const QImage& image);
	QString getAvatarPath();

	void addImage(const QString& name, const QImage& image);
	void removeImage(const QString& name);
	void replaceImage(const QString& name, const QImage& image);

	void cacheImage(const QString& path, const QByteArray& imageData);
	void freeImage(const QString& path);

signals:
	void event_finish();

private:
	void loadImages();

	void loadAvatar();
};

#endif // RESOURCEMANAGER_H
