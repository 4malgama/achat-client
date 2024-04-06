#include "init_profile_packet.h"
#include "../packets.h"
#include "../stream_parser.h"

#include <QJsonDocument>
#include <QJsonObject>

#include <QDebug>

InitProfilePacket::InitProfilePacket()
    : IPacket(INIT_PROFILE_PACKET)
{

}

InitProfilePacket::~InitProfilePacket() { }

QByteArray InitProfilePacket::prepareToSend() const
{
    return QByteArray();
}

void InitProfilePacket::prepareToRead(const QByteArray& data)
{
    StreamParser sp(data);
    quint16 length = sp.parseUInt16();
    if (length == 0)
        return;
    
    bool ok = false;
    QString json = sp.parseString(length, &ok);

    if (!ok) return;

    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject obj = doc.object();

    if (obj.contains("profile_info"))
    {
        QJsonObject profileInfo = obj.value("profile_info").toObject();

        profileData.insert("login", profileInfo.value("login").toString());
        profileData.insert("email", profileInfo.value("email").toString());
        profileData.insert("first_name", profileInfo.value("first_name").toString());
        profileData.insert("sur_name", profileInfo.value("sur_name").toString());
        profileData.insert("patronymic", profileInfo.value("patronymic").toString());
        profileData.insert("post", profileInfo.value("post").toString());
        profileData.insert("description", profileInfo.value("description").toString());
        profileData.insert("score", profileInfo.value("score").toInt());
        profileData.insert("balance", profileInfo.value("balance").toInt());
        profileData.insert("cash", profileInfo.value("cash").toInt());
        profileData.insert("registration_date", profileInfo.value("registration_date").toInt());
    }

    if (obj.contains("private_settings"))
    {
        QJsonObject privateSettings = obj.value("private_settings").toObject();

        profileData.insert("hide_forwards", privateSettings.value("hide_forwards").toBool());
        profileData.insert("see_avatar", privateSettings.value("see_avatar").toString());
        profileData.insert("send_messages", privateSettings.value("send_messages").toString());
        profileData.insert("see_description", privateSettings.value("see_description").toString());
        profileData.insert("can_invite_groups", privateSettings.value("can_invite_groups").toString());
        profileData.insert("send_friend_request", privateSettings.value("send_friend_request").toString());
        profileData.insert("see_online_status", privateSettings.value("see_online_status").toString());
        profileData.insert("see_post", privateSettings.value("see_post").toString());
    }

    if (obj.contains("permissions"))
    {
        //todo
    }
}