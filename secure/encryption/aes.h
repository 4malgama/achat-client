#ifndef AES_H
#define AES_H

#include <QByteArray>

class AES
{
public:
	enum Mode
	{
		ECB_128,
		ECB_256,
		CBC_128,
		CBC_256
	};

	AES(Mode mode);
	AES(Mode mode, const QByteArray& iv);
	~AES();

	QByteArray encrypt();
	QByteArray encrypt(const QByteArray& data);
	QByteArray encrypt(const QByteArray& data, const QByteArray& key);
	static QByteArray encrypt(Mode mode, const QByteArray& data, const QByteArray& key);
	static QByteArray encrypt(Mode mode, const QByteArray& data, const QByteArray& key, const QByteArray& iv);

	QByteArray decrypt(const QByteArray& data);
	QByteArray decrypt(const QByteArray& data, const QByteArray& key);
	static QByteArray decrypt(Mode mode, const QByteArray& data, const QByteArray& key);
	static QByteArray decrypt(Mode mode, const QByteArray& data, const QByteArray& key, const QByteArray& iv);

	Mode getMode() const;
	void setMode(Mode newMode);

	QByteArray getData() const;
	void setData(const QByteArray &newData);

	QByteArray getKey() const;
	void setKey(const QByteArray &newKey);

	QByteArray IV() const;
	void setIV(const QByteArray &newIV);

private:
	Mode m_Mode;
	QByteArray m_Data;
	QByteArray m_Key;
	QByteArray m_IV;

	void handleError();
	const void* getEVP();
};

#endif // AES_H
