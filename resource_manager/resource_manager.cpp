#include "resource_manager.h"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>


namespace resourcemanager
{
	void load()
	{
		ResourceManager::instance().load();
	}
}

QByteArray HashPassword(const QString& password);



ResourceManager& ResourceManager::instance()
{
	static ResourceManager instance;
	return instance;
}

ResourceManager::ResourceManager(QObject *parent)
	: QObject(parent)
{
}

bool ResourceManager::isLoaded() const
{
	return m_loaded;
}

void ResourceManager::load()
{
	m_cachePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/cache/";

	QDir dir(m_cachePath);
	if (!dir.exists())
		dir.mkpath(".");

	scanImages();

	m_loaded = true;
	emit event_finish();
}

QImage ResourceManager::image(const QString& name)
{
	if (m_imageCache.contains(name))
		return m_imageCache[name];

	if (!m_imagePaths.contains(name))
		return QImage();

	QString path = m_imagePaths[name];

	QImage img(path);

	if (img.isNull())
		return QImage();

	m_imageCache.insert(name, img);
	return img;
}

void ResourceManager::registerImage(const QString &name, const QString &path)
{
	m_imagePaths.insert(name, path);
}

QImage ResourceManager::avatar() const
{
	return QImage(avatarPath());
}

void ResourceManager::setAvatar(const QImage& image)
{
	image.save(avatarPath(), "JPG");
}

void ResourceManager::setAvatarData(const QByteArray& imageData)
{
	QFile file(avatarPath());

	if (file.open(QIODevice::WriteOnly))
	{
		file.write(imageData);
		file.close();
	}
}

QString ResourceManager::avatarPath() const
{
	return m_userPath + "/avatar.jpg";
}

void ResourceManager::addImage(const QString& name, const QImage& image)
{
	m_imageCache.insert(name, image);
}

void ResourceManager::removeImage(const QString& name)
{
	m_imagePaths.remove(name);
	m_imageCache.remove(name);
}

void ResourceManager::cacheImage(const QString& path, const QByteArray& imageData)
{
	QString fullPath = m_cachePath + "/images/" + path;

	QFileInfo fi(fullPath);
	QDir dir(fi.absolutePath());

	if (!dir.exists())
		dir.mkpath(".");

	QFile file(fullPath);

	if (file.open(QIODevice::WriteOnly))
	{
		file.write(imageData);
		file.close();
	}

	QString name = fi.baseName();
	m_imagePaths.insert(name, fullPath);
}

void ResourceManager::freeImage(const QString& path)
{
	QString fullPath = m_cachePath + "/images/" + path;
	QFile::remove(fullPath);

	QFileInfo fi(fullPath);
	QString name = fi.baseName();
	m_imagePaths.remove(name);
	m_imageCache.remove(name);
}


void ResourceManager::loadImages()
{
	loadImage("gpt.jpg");
}

void ResourceManager::loadImage(const QString &path)
{
	QString fullPath = m_cachePath + "/images/" + path;

	if (!QFile::exists(fullPath))
			return;

	QFileInfo fi(fullPath);

	QImage img(fullPath);
	if (!img.isNull())
		m_imageCache.insert(fi.baseName(), img);
}

void ResourceManager::scanImages()
{
	QDir imgDir(m_cachePath + "/images");
	if (!imgDir.exists())
		return;

	QStringList filters;
	filters << "*.png" << "*.jpg" << "*.jpeg";

	QFileInfoList files = imgDir.entryInfoList(filters, QDir::Files);

	for (const QFileInfo& fi : files)
	{
		QString name = fi.baseName();
		QString path = fi.absoluteFilePath();

		m_imagePaths.insert(name, path);
	}
}

void ResourceManager::initUser(quint64 uid, const QString &token)
{
	if (uid == 0 || uid == m_uid)
		return;

	m_uid = uid;
	m_userPath = m_cachePath + "users/user_" + QString::number(m_uid);

	QDir dir(m_userPath);
	if (!dir.exists())
		dir.mkpath(".");

	if (!token.isEmpty())
	{
		QFile file(m_userPath + "/login.cfg");
		if (file.open(QIODevice::WriteOnly))
		{
			file.write(token.toUtf8());
			file.close();
		}
	}
}

QString ResourceManager::getToken()
{
	QFile file(m_userPath + "/login.cfg");
	if (!file.exists() || !file.open(QIODevice::ReadOnly))
		return QString();

	QByteArray token = file.readAll();
	file.close();

	return QString::fromUtf8(token);
}
