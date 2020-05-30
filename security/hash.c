/*
 * hash.c
 *
 *  Created on: May 28, 2020
 *      Author: khubaibumer
 */

#include <thor.h>
#include <openssl/evp.h>

int __compute_hash(void *ptr, const char *key, char **hash) {

	size_t len = strlen(key);
	EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
	const EVP_MD *md = EVP_md5();
	unsigned char md_value[EVP_MAX_MD_SIZE];
	unsigned int md_len, i;
	EVP_DigestInit_ex(mdctx, md, NULL);
	EVP_DigestUpdate(mdctx, key, len);
	EVP_DigestFinal_ex(mdctx, md_value, &md_len);
	EVP_MD_CTX_free(mdctx);

	*hash = calloc(1 + EVP_MAX_MD_SIZE, sizeof(char));

	for (i = 0; i < md_len; i++) {
		snprintf(&((*hash)[i * 2]), 16 * 2, "%02x", md_value[i]);
	}

	log.i("key: %s\nhash: %s\n", key, *hash);

	return 0;
}
