#include "aes.h"
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>



namespace aes
{
	bool initialized = false;

	void init()
	{
		OpenSSL_add_all_algorithms();
		initialized = true;
	}

	void free()
	{
		if (initialized)
			EVP_cleanup();
	}
}

AES::AES(AES::Mode mode)
{
	m_Mode = mode;
}

AES::AES(Mode mode, const QByteArray &iv)
{
	m_Mode = mode;
	m_IV = iv;
}

AES::~AES()
{
}

QByteArray AES::encrypt()
{
	return encrypt(m_Data, m_Key);
}

QByteArray AES::encrypt(const QByteArray &data)
{
	return encrypt(data, m_Key);
}

QByteArray AES::encrypt(const QByteArray &data, const QByteArray &key)
{
	EVP_CIPHER_CTX* ctx;
	int len;
	int cipher_len;
	unsigned char* cipher = new unsigned char [data.size() + EVP_MAX_BLOCK_LENGTH];

	if (!(ctx = EVP_CIPHER_CTX_new()))
		handleError();

	const unsigned char* iv = (m_Mode == AES::CBC_128 || m_Mode == AES::CBC_256 ? reinterpret_cast<const unsigned char*>(m_IV.data()) : NULL);

	if (EVP_EncryptInit_ex(ctx, static_cast<const EVP_CIPHER*>(getEVP()), NULL, reinterpret_cast<const unsigned char*>(key.data()), iv) != 1)
		handleError();

	if (EVP_EncryptUpdate(ctx, cipher, &len, reinterpret_cast<const unsigned char*>(data.data()), data.size()) != 1)
		handleError();

	cipher_len = len;

	if (EVP_EncryptFinal_ex(ctx, cipher + len, &len) != 1)
		handleError();

	cipher_len += len;

	EVP_CIPHER_CTX_free(ctx);
	QByteArray result = QByteArray(reinterpret_cast<const char*>(cipher), cipher_len);
	delete[] cipher;
	return result;
}

/* static */ QByteArray AES::encrypt(Mode mode, const QByteArray &data, const QByteArray &key)
{
	AES aes(mode);
	aes.setData(data);
	aes.setKey(key);
	return aes.encrypt();
}

QByteArray AES::encrypt(Mode mode, const QByteArray &data, const QByteArray &key, const QByteArray &iv)
{
	AES aes(mode, iv);
	aes.setData(data);
	aes.setKey(key);
	return aes.encrypt();
}

QByteArray AES::decrypt(const QByteArray &data)
{
	return decrypt(data, m_Key);
}

QByteArray AES::decrypt(const QByteArray &data, const QByteArray &key)
{
	EVP_CIPHER_CTX* ctx;
	int len;
	int plain_len;
	unsigned char* plain = new unsigned char [data.size() + EVP_MAX_BLOCK_LENGTH];

	if (!(ctx = EVP_CIPHER_CTX_new()))
		handleError();

	const unsigned char* iv = (m_Mode == AES::CBC_128 || m_Mode == AES::CBC_256 ? reinterpret_cast<const unsigned char*>(m_IV.data()) : NULL);

	if (EVP_DecryptInit_ex(ctx, static_cast<const EVP_CIPHER*>(getEVP()), NULL, reinterpret_cast<const unsigned char*>(key.data()), iv) != 1)
		handleError();

	if (EVP_DecryptUpdate(ctx, plain, &len, reinterpret_cast<const unsigned char*>(data.data()), data.size()) != 1)
		handleError();

	plain_len = len;

	if (EVP_DecryptFinal_ex(ctx, plain + len, &len) != 1)
		handleError();

	plain_len += len;

	EVP_CIPHER_CTX_free(ctx);
	QByteArray result = QByteArray(reinterpret_cast<const char*>(plain), plain_len);
	delete[] plain;
	return result;
}

QByteArray AES::decrypt(Mode mode, const QByteArray &data, const QByteArray &key)
{
	AES aes(mode);
	return aes.decrypt(data, key);
}

QByteArray AES::decrypt(Mode mode, const QByteArray &data, const QByteArray &key, const QByteArray &iv)
{
	AES aes(mode, iv);
	return aes.decrypt(data, key);
}

AES::Mode AES::getMode() const
{
	return m_Mode;
}

void AES::setMode(AES::Mode newMode)
{
	m_Mode = newMode;
}

QByteArray AES::getData() const
{
	return m_Data;
}

void AES::setData(const QByteArray &newData)
{
	m_Data = newData;
}

QByteArray AES::getKey() const
{
	return m_Key;
}

void AES::setKey(const QByteArray &newKey)
{
	m_Key = newKey;
}

QByteArray AES::IV() const
{
	return m_IV;
}

void AES::setIV(const QByteArray &newIV)
{
	m_IV = newIV;
}

void AES::handleError()
{
	FILE* fp;
	fopen_s(&fp, "SecureErrors.log", "r+");
	fwrite("\n", 1, 1, fp);
	ERR_print_errors_fp(fp);
	fclose(fp);
	throw 1;
}

const void *AES::getEVP()
{
	switch(m_Mode)
	{
		case ECB_128:
			return EVP_aes_128_ecb();
		case ECB_256:
			return EVP_aes_256_ecb();
		case CBC_128:
			return EVP_aes_128_cbc();
		case CBC_256:
			return EVP_aes_256_cbc();
		default:
			return nullptr;
	}
}
