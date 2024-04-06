#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QObject>
#include <QMap>
#include <QImage>


class ResourceManager : public QObject
{
	Q_OBJECT

	bool loaded;
	quint64 current_uid;
	QString m_Path;
	QString m_UserPath;
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
	void setAvatarData(const QByteArray& imageData);
	QString getAvatarPath();

	void addImage(const QString& name, const QImage& image);
	void removeImage(const QString& name);
	void replaceImage(const QString& name, const QImage& image);

	void cacheImage(const QString& path, const QByteArray& imageData);
	void freeImage(const QString& path);

	void initUser(quint64 uid, bool remember = false, const QString& login = QString(), const QString& password = QString());
	QPair<QString, QString> getAutoLoginData();

signals:
	void event_finish();

private:
	void loadImages();
};

#endif // RESOURCEMANAGER_H
