#include "gpt_service.h"
#include "request_builder.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace settings
{
	extern WebService::ProxyData proxyData;
}

namespace console
{
	void writeLine(const QString& text);
}

GPTService::GPTService(const QString token, QObject *parent)
	: WebService{parent}
{
	this->token = token;
}

QString GPTService::ask(const QString &text)
{
	RequestBuilder builder("https://api.openai.com/v1/chat/completions");
	builder.setHeader("Content-Type", "application/json");
	builder.setHeader("Authorization", "Bearer " + token);

	QJsonObject message;
	message.insert("role", "user");
	message.insert("content", text);

	QJsonArray messages;
	messages.append(message);

	QJsonObject json;
	json.insert("model", "gpt-3.5-turbo");
	json.insert("messages", messages);

	QByteArray data = QJsonDocument(json).toJson(QJsonDocument::Compact);

	QByteArray response = post(builder, data);

	return getAnswer(response);
}

void GPTService::loadProxy()
{
	setProxy(settings::proxyData.ip, settings::proxyData.port, settings::proxyData.user, settings::proxyData.password);
}

QString GPTService::getAnswer(const QByteArray &jsonData)
{
	QJsonDocument doc = QJsonDocument::fromJson(jsonData);
	QJsonObject json = doc.object();

	QJsonArray choices = json.value("choices").toArray();
	QJsonObject obj = choices.first().toObject();
	QJsonObject message = obj.value("message").toObject();
	QString content = message.value("content").toString();
	console::writeLine(jsonData);
	return content;
}
