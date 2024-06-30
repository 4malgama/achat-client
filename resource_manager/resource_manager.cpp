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

QByteArray HashPassword(const QString& password);

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
	return QImage(m_UserPath + "\\avatar.jpg");
}

void ResourceManager::setAvatar(const QImage& image)
{
	QImage avatar = image;
	avatar.save(m_UserPath + "\\avatar.jpg", "JPG");
}

void ResourceManager::setAvatarData(const QByteArray& imageData)
{
	QFile file(m_UserPath + "\\avatar.jpg");
	file.open(QIODevice::WriteOnly);
	file.write(imageData);
	file.close();
}

QString ResourceManager::getAvatarPath()
{
	return m_UserPath + "\\avatar.jpg";
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
	loadImage("gpt.jpg");
}

void ResourceManager::loadImage(const QString &path)
{
	QFileInfo fi(m_Path + "images\\" + path);

	QDir dir(fi.absolutePath());

	if (!dir.exists())
		return;

	addImage(fi.baseName(), QImage(fi.absoluteFilePath()));
}

void ResourceManager::initUser(quint64 uid, bool remember, const QString& login, const QString& password)
{
	if (uid == 0 || uid == current_uid)
		return;

	current_uid = uid;
	m_UserPath = m_Path + "users\\user_" + QString::number(current_uid) + "\\";

	QDir dir(m_UserPath);

	if (!dir.exists())
		dir.mkpath(".");

	if (remember && !login.isEmpty() && !password.isEmpty())
	{
		QByteArray token = (login + ":" + HashPassword(password)).toUtf8();

		QFile file(m_UserPath + "\\auto_login.bin");
		if (file.open(QIODevice::WriteOnly))
		{
			file.write(token);
			file.close();
		}
	}
}

void ResourceManager::initUser(quint64 uid, const QString &token)
{
	if (uid == 0 || uid == current_uid)
		return;

	current_uid = uid;

	m_UserPath = m_Path + "users\\user_" + QString::number(current_uid) + "\\";

	QDir dir(m_UserPath);

	if (!dir.exists())
		dir.mkpath(".");

	if (!token.isEmpty())
	{
		QFile file(m_UserPath + "\\login.cfg");
		if (file.open(QIODevice::WriteOnly))
		{
			file.write(token.toUtf8());
			file.close();
		}
	}
}

QPair<QString, QString> ResourceManager::getAutoLoginData()
{
	QFile file(m_UserPath + "\\auto_login.bin");
	if (!file.exists() || !file.open(QIODevice::ReadOnly))
		return QPair<QString, QString>();

	QByteArray token = file.read(1024);
	file.close();

	int i = token.indexOf(':');
	if (i == -1)
		return QPair<QString, QString>();

	return QPair<QString, QString>(token.left(i), token.mid(i + 1));
}

QString ResourceManager::getToken()
{
	QFile file(m_UserPath + "\\login.cfg");
	if (!file.exists() || !file.open(QIODevice::ReadOnly))
		return QString();

	QByteArray token = file.readAll();
	file.close();

	return QString::fromUtf8(token);
}
