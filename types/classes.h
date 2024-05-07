#ifndef CLASSES_H
#define CLASSES_H

#include "types_variables.h"
#include <QImage>

struct ProfileData
{
	uint64 uid;
	QImage avatar;
	QString fname;
	QString sname;
	QString mname;
	QString post;
};

struct InitChatData
{
	uint64 id;
	bool isGroup;
	ProfileData user;
};

struct ChatMessage
{

};

struct ChatData
{
	InitChatData data;
	QList<ChatMessage> messages;
};

#endif // CLASSES_H
