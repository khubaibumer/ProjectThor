/*
 * authorize.c
 *
 *  Created on: May 27, 2020
 *      Author: khubaibumer
 */

#include <thor.h>
#include <sys/socket.h>

DECLARE_SYMBOL(const char, *usr) = "admin";
DECLARE_SYMBOL(const char, *psswd) = "admin";
DECLARE_SYMBOL(const size_t, plen) = sizeof("admin");

typedef struct {
	const char *name;
	const char *pass;
	const size_t plen;
	const int mode;
} valid_users_t;

static const valid_users_t valid_users[] = {
		{ "admin", "admin", sizeof("admin"), ROOT_USR },
		{ "user", "user", sizeof("user"), DFL_USR },
		{ "audit", "audit",	sizeof("audit"), ELVT_USR },
		{ "admin@admin.com", "admin123" , sizeof("admin@admin.com"),  ROOT_USR },
		{ "merchant@merchant.com", "merchant123" , sizeof("merchant@merchant.com"),  DFL_USR },
};

DECLARE_SYMBOL(const size_t, usr_list_len) = sizeof(valid_users)
		/ sizeof(valid_users_t);

int get_user_mode(void *ptr, const char *id, const char *pas, const size_t ilen,
		const size_t _plen) {

	if (CAST(ptr)->db.is_open == 0) {
		for (int i = 0; i < usr_list_len; i++) {
			if ((ilen == valid_users[i].plen) && _plen == valid_users[i].plen) {
				if ((memcmp(id, valid_users[i].name, ilen - 1) == 0)
						&& (memcmp(pas, valid_users[i].pass, ilen - 1) == 0)) {
					return valid_users[i].mode;
				}
			}
		}
	} else {
		// Use DB for authorization
		return (CAST(ptr)->db.get_role(ptr, id, pas));
	}
	return -1;
}

int register_users() {

	for (int i = 0; i < usr_list_len; i++) {
		CAST(THIS)->db.psswd_db.creat_usr(THIS, valid_users[i].name,
				valid_users[i].pass, valid_users[i].mode);
	}

	return 0;
}

void remove_escape(char *name, size_t *inlen, char *pass, size_t *iplen) {

	size_t _nlen = *inlen;
	size_t _plen = *iplen;

	for(int i = _nlen; i >= 0; i--) {
		if(name[i] == '\n' || name[i] == '\r' /*|| name[i] == '\s'*/)
			name[i] = '\0';
	}

	for(int i = _plen; i >= 0; i--) {
		if(pass[i] == '\n' || pass[i] == '\r' /*|| pass[i] == '\s'*/)
			pass[i] = '\0';
	}

	*inlen = strlen(name);
	*iplen = strlen(pass);
}

int __auth(void *ptr, int fd) {

	if(CAST(ptr)->use_ssl) {

		BIO *sbio = BIO_new_socket(fd, BIO_NOCLOSE);
		SSL *ssl = SSL_new(CAST(THIS)->ssl_tls.ctx);
		SSL_set_bio(ssl, sbio, sbio);
		if(SSL_accept(ssl) <= 0) {
			perror("Error");
		}
		SSL_do_handshake(ssl);

		CAST(ptr)->tmp_cli_info.tssl = SSL_dup(ssl);
		CAST(ptr)->tmp_cli_info.tbio = sbio;

		char buf[1024] = { };
		SSL_write(CAST(ptr)->tmp_cli_info.tssl, "Enter Credentials: \n", sizeof("Enter Credentials: \n"));
		SSL_read(CAST(ptr)->tmp_cli_info.tssl, buf, 1024);

		log.v("Got: %s\n", buf);

		if(strlen(buf) > 0) {
			if(strstr(buf, ",")) {
				char *name = strtok(buf, ",");
				char *pass = strtok(NULL, ",");

				size_t inlen = name == NULL ? 0 : strlen(name);
				size_t iplen = pass == NULL ? 0 : strlen(pass);

				remove_escape(name, &inlen, pass, &iplen);

				if(inlen && iplen) {
					return get_user_mode(ptr, name, pass, inlen, iplen);
				}
			}
		}
	} else {
		char name[1024] = { };
		char pass[1024] = { };
		int wb = send(fd, "Enter Username: ", sizeof("Enter Username: "), 0);
		wb = recv(fd, name, 1024, 0);
		wb = send(fd, "Enter Password: ", sizeof("Enter Password: "), 0);
		wb = recv(fd, pass, 1024, 0);

		if (wb < 0)
			perror("something wrong!");

		size_t unlen = strlen(name);
		size_t uplen = strlen(pass);

		if(name[unlen-1] == '\n')
			name[--unlen] = '\0';

		if(pass[uplen-1] == '\n')
			pass[--uplen] = '\0';

		return get_user_mode(ptr, name, pass, unlen, uplen);
	}
	return -1;
}

