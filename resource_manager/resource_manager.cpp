#include "resource_manager.h"

#include <QApplication>
#include <QFile>
#include <QFileInfo>
#include <QDir>

#include <QDebug>

namespace resourcemanager
{
	void load()
	{
		ResourceManager::instance().load();
	}
}

ResourceManager& ResourceManager::instance()
{
	static ResourceManager instance;
	return instance;
}

ResourceManager::ResourceManager(QObject *parent)
	: QObject{parent}
{ }

bool ResourceManager::isLoaded()
{
	return loaded;
}

void ResourceManager::load()
{
	loaded = false;
	m_Path = QString(qApp->applicationDirPath() + "\\cache\\");
	QDir dir(m_Path);

	if (!dir.exists())
	{
		dir.mkpath(".");
		return;
	}

	loadImages();

	emit event_finish();
	loaded = true;
}

QImage ResourceManager::getImage(const QString& name)
{
	return images[name];
}

QImage ResourceManager::getAvatar()
{
	return getImage("$avatar");
}

void ResourceManager::setAvatar(const QImage& image)
{
	replaceImage("$avatar", image);
}

QString ResourceManager::getAvatarPath()
{
	return m_Path + "images\\user\\avatar.jpg";
}

void ResourceManager::addImage(const QString& name, const QImage& image)
{
	if (images.contains(name))
		return;
	images.insert(name, image);
}

void ResourceManager::removeImage(const QString& name)
{
	if (!images.contains(name))
		return;
	images.remove(name);
}

void ResourceManager::replaceImage(const QString& name, const QImage& image)
{
	removeImage(name);
	addImage(name, image);
}


void ResourceManager::cacheImage(const QString& path, const QByteArray& imageData)
{
	QFileInfo fi(m_Path + "images\\" + path);

	QDir dir(fi.absolutePath());

	if (!dir.exists())
		dir.mkpath(".");

	QFile file(dir.path() + "\\" + fi.fileName());
	file.open(QIODevice::WriteOnly);
	file.write(imageData);
	file.close();
}

void ResourceManager::freeImage(const QString& path)
{
	QFileInfo fi(m_Path + "images\\" + path);

	QDir dir(fi.absolutePath());

	if (!dir.exists())
		return;

	QFile file(dir.path() + "\\" + fi.fileName());
	file.remove();
	file.close();
}


void ResourceManager::loadImages()
{
	loadAvatar();
}

void ResourceManager::loadAvatar()
{
	QDir dir(m_Path + "images\\user\\");

	if (!dir.exists())
	{
		dir.mkpath(".");
		return;
	}

	QImage image;
	image.load(dir.path() + "\\avatar.jpg");
	addImage("$avatar", image);
}