#ifndef GPTSERVICE_H
#define GPTSERVICE_H

#include "web_service.h"

class GPTService : public WebService
{
	Q_OBJECT

	QString token;

public:
	explicit GPTService(const QString token, QObject *parent = nullptr);

	QString ask(const QString& text);
	void loadProxy();

private:
	QString getAnswer(const QByteArray& jsonData);
};

#endif // GPTSERVICE_H
