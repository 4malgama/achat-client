#ifndef REQUESTBUILDER_H
#define REQUESTBUILDER_H

#include <QVariant>
#include <QNetworkRequest>

class RequestBuilder
{
	QString url;
	QHash<QString, QVariant> headers;

public:
	RequestBuilder();
	RequestBuilder(const QString& url);

	void setUrl(const QString& url);
	void setHeader(const QString& key, const QVariant& value);

	QNetworkRequest build() const;
};

#endif // REQUESTBUILDER_H
