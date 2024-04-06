#ifndef JSONUTILS_H
#define JSONUTILS_H

#include <QHash>

class QVariant;
class QJsonDocument;


class JsonUtils
{
public:
	static QJsonDocument hashmapToJson(const QHash<QString, QVariant>& hashmap);
	static QHash<QString, QVariant> jsonToHashmap(const QJsonDocument& doc);
	static QString jsonToString(const QJsonDocument& doc);
};

#endif // JSONUTILS_H
