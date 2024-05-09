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

struct ChatMessageUser
{
	uint64 uid;
	QString fname;
	QString sname;
	QString mname;
};

struct ChatMessageAttachment
{

};

struct ChatMessage
{
	uint64 id;
	uint64 timestamp;
	uint64 replyId;
	uint64 forwardId;
	QString content;
	ChatMessage* replyMsg;
	ChatMessage* forwardMsg;
	ChatMessageUser user;
	QList<ChatMessageAttachment> attachments;
};

struct ChatData
{
	bool initialized = false;
	InitChatData data;
	QList<ChatMessage> messages;
};

#endif // CLASSES_H
