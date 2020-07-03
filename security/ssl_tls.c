/*
 * ssl_tls.c
 *
 *  Created on: May 28, 2020
 *      Author: khubaibumer
 */

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <resolv.h>
#include <thor.h>

SSL_CTX* init_server_ctx(void) {

	SSL_METHOD *method;
	SSL_CTX *ctx;
	OpenSSL_add_all_algorithms(); /* load & register all cryptos, etc. */
	SSL_load_error_strings(); /* load all error messages */
	method = TLS_server_method(); /* create new server-method instance */
	ctx = SSL_CTX_new(method); /* create new context from method */
	if (ctx == NULL) {
		ERR_print_errors_fp(stderr);
		abort();
	}
	return ctx;
}

void load_certs(SSL_CTX *ctx, char *CertFile, char *KeyFile) {
	/* set the local certificate from CertFile */
	if (SSL_CTX_use_certificate_file(ctx, CertFile, SSL_FILETYPE_PEM) <= 0) {
		ERR_print_errors_fp(log.get());
		abort();
	}
	/* set the private key from KeyFile (may be the same as CertFile) */
	if (SSL_CTX_use_PrivateKey_file(ctx, KeyFile, SSL_FILETYPE_PEM) <= 0) {
		ERR_print_errors_fp(log.get());
		abort();
	}
	/* verify private key */
	if (!SSL_CTX_check_private_key(ctx)) {
		log.f("%s", "Private key does not match the public certificate\n");
		abort();
	}
}

void show_certs(SSL *ssl) {

	X509 *cert;
	cert = SSL_get_peer_certificate(ssl); /* Get certificates (if available) */
	if (cert != NULL) {
		char *line;
		printf("Server certificates:\n");
		line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
		printf("Subject: %s\n", line);
		free(line);
		line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
		printf("Issuer: %s\n", line);
		free(line);
		X509_free(cert);
	} else
		printf("No certificates.\n");
}

int __ssl_initialize(void *ptr) {

	CAST(ptr)->ssl_tls.ctx = init_server_ctx();
	load_certs(CAST(ptr)->ssl_tls.ctx, CAST(ptr)->ssl_tls.certifiate, CAST(ptr)->ssl_tls.certifiate);

	CAST(ptr)->use_ssl = 1;
	return 0;
}
