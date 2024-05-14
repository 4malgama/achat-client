#include "certification_manager.h"
#include <openssl/x509.h>
#include <openssl/pem.h>


bool CertificationManager::verifyCertificate(const QByteArray &data)
{
	BIO* bio = BIO_new_mem_buf(data.data(), data.size());
	if (!bio) return false;

	X509* cert = nullptr;
	PEM_read_bio_X509(bio, &cert, nullptr, nullptr);
	BIO_free(bio);

	if (!cert) return false;

	X509_STORE* store = X509_STORE_new();
	if (!store) {
		X509_free(cert);
		return false;
	}

	X509_STORE_add_cert(store, cert);

	X509_STORE_CTX* ctx = X509_STORE_CTX_new();
	if (!ctx) {
		X509_free(cert);
		X509_STORE_free(store);
		return false;
	}

	X509_STORE_CTX_init(ctx, store, cert, nullptr);
	int result = X509_verify_cert(ctx);

	X509_STORE_CTX_free(ctx);
	X509_STORE_free(store);
	X509_free(cert);

	return result == 1;
}
