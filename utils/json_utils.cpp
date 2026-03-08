#include "json_utils.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariant>


QJsonDocument JsonUtils::hashmapToJson(const QHash<QString, QVariant>& hashmap)
{
    return QJsonDocument(QJsonObject::fromVariantHash(hashmap));
}


QHash<QString, QVariant> JsonUtils::jsonToHashmap(const QJsonDocument& doc)
{
    QHash<QString, QVariant> hashmap;

    if (doc.isNull() || doc.isEmpty())
        return hashmap;

    QJsonObject obj = doc.object();

    for (QJsonObject::const_iterator it = obj.constBegin(); it != obj.constEnd(); ++it)
    {
        hashmap.insert(it.key(), it.value());
    }

    return hashmap;
}


QString JsonUtils::jsonToString(const QJsonDocument& doc)
{
    return doc.toJson(QJsonDocument::Compact);
}
