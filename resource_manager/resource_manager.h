#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QObject>
#include <QMap>
#include <QImage>

//TODO:
///
/// \brief The ResourceManager class
/// ResourceManager
/// ImageCache
/// UserStorage
/// AuthStorage
///

class ResourceManager : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(ResourceManager)

	bool m_loaded = false;
	quint64 m_uid = 0;
	QString m_cachePath;
	QString m_userPath;
	QMap<QString, QImage> m_imageCache;
	QMap<QString, QString> m_imagePaths;

private:
	explicit ResourceManager(QObject *parent = nullptr);
	void loadImages();
	void loadImage(const QString& path);
	void scanImages();

public:
	static ResourceManager& instance();

	void load();
	bool isLoaded() const;

	QImage image(const QString& name);

	void registerImage(const QString& name, const QString& path);
	void addImage(const QString& name, const QImage& image);
	void removeImage(const QString& name);

	void cacheImage(const QString& path, const QByteArray& imageData);
	void freeImage(const QString& path);

	QImage avatar() const;
	void setAvatar(const QImage& image);
	void setAvatarData(const QByteArray& imageData);

	QString avatarPath() const;

	void initUser(quint64 uid, const QString& token = QString());

	QString getToken();

signals:
	void event_finish();

};

#endif // RESOURCEMANAGER_H
