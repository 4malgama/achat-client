#include "json_utils.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariant>


QJsonDocument JsonUtils::hashmapToJson(const QHash<QString, QVariant>& hashmap)
{
    return QJsonDocument::fromVariant(hashmap);
}


QHash<QString, QVariant> JsonUtils::jsonToHashmap(const QJsonDocument& doc)
{
    if (!doc.isObject())
        return {};

    return doc.object().toVariantHash();
}


QString JsonUtils::jsonToString(const QJsonDocument& doc)
{
    return doc.toJson(QJsonDocument::Compact);
}
