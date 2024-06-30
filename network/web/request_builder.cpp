#include "request_builder.h"

RequestBuilder::RequestBuilder()
{

}

RequestBuilder::RequestBuilder(const QString &url)
{
	this->url = url;
}

void RequestBuilder::setUrl(const QString &url)
{
	this->url = url;
}

void RequestBuilder::setHeader(const QString &key, const QVariant &value)
{
	if (headers.contains(key))
	{
		headers[key] = value;
	}
	else
	{
		headers.insert(key, value);
	}
}

QNetworkRequest RequestBuilder::build() const
{
	QNetworkRequest req;
	req.setUrl(url);

	for (auto i = headers.cbegin(); i != headers.cend(); ++i)
		req.setRawHeader(i.key().toUtf8(), i.value().toString().toUtf8());

	return req;
}
