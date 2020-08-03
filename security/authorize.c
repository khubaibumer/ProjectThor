/*
 * authorize.c
 *
 *  Created on: May 27, 2020
 *      Author: khubaibumer
 */

#include <thor.h>
#include <sys/socket.h>

extern uint8_t __is_logged(void *ptr, const char *name);

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
		{ "admin", "admin",	sizeof("admin"), ROOT_USR },
		{ "user", "user", sizeof("user"), DFL_USR },
		{ "audit", "audit",	sizeof("audit"), ELVT_USR },
		{ "admin@admin.com", "admin123", sizeof("admin@admin.com"), ROOT_USR },
		{ "merchant@merchant.com", "merchant123", sizeof("merchant@merchant.com"), ELVT_USR },
		{ "checkoutuser", "user123", sizeof("checkoutuser"), DFL_USR },
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

void remove_escape(char *name, size_t *inlen) {

	size_t _nlen = *inlen;

	for (int i = _nlen; i >= 0; i--) {
		if (name[i] == '\n' || name[i] == '\r' /*|| name[i] == '\s'*/)
			name[i] = '\0';
	}

	*inlen = strnlen(name, *inlen);
}

int __auth(void *ptr, int fd) {

	if (CAST(ptr)->use_ssl) {

		BIO *sbio = BIO_new_socket(fd, BIO_NOCLOSE);
		SSL *ssl = SSL_new(CAST(THIS)->ssl_tls.ctx);
		SSL_set_bio(ssl, sbio, sbio);
		if (SSL_accept(ssl) <= 0) {
			ERR_print_errors(sbio);
		}
		SSL_do_handshake(ssl);

		CAST(ptr)->tmp_cli_info.tssl = SSL_dup(ssl);
		CAST(ptr)->tmp_cli_info.tbio = sbio;

		char buf[1025] = { };
		SSL_write(CAST(ptr)->tmp_cli_info.tssl, "auth,who,<$#EOT#$>\n",
				sizeof("auth,who,<$#EOT#$>\n"));
		SSL_read(CAST(ptr)->tmp_cli_info.tssl, buf, 1024);

		log.v("Got: %s\n", buf);

		if (strnlen(buf, 1024) > 0) {
			if (strstr(buf, ",")) {
				char *cmd = strtok(buf, ",");
				char *name = strtok(NULL, ",");
				char *pass = strtok(NULL, ",");

				if(strncmp(cmd, "auth", 4) != 0)
					return -1;

				size_t inlen = name == NULL ? 0 : strlen(name);
				size_t iplen = pass == NULL ? 0 : strlen(pass);

				remove_escape(name, &inlen);
				remove_escape(pass, &iplen);

				CAST(ptr)->tmp_cli_info.log_bit = __is_logged(ptr, name);

				if (inlen && iplen) {
					return get_user_mode(ptr, name, pass, inlen, iplen);
				}
			}
		}
	} else {
		char name[1025] = { };
		char pass[1025] = { };
		int wb = send(fd, "Enter Username: ", sizeof("Enter Username: "), 0);
		if (wb < 0)
			perror("something wrong!");
		wb = recv(fd, name, 1024, 0);
		if (wb < 0)
			perror("something wrong!");
		wb = send(fd, "Enter Password: ", sizeof("Enter Password: "), 0);
		if (wb < 0)
			perror("something wrong!");
		wb = recv(fd, pass, 1024, 0);
		if (wb < 0)
			perror("something wrong!");


		size_t unlen = strnlen(name, 1024);
		size_t uplen = strnlen(pass, 1024);

		if (name[unlen - 1] == '\n')
			name[--unlen] = '\0';

		if (pass[uplen - 1] == '\n')
			pass[--uplen] = '\0';

		return get_user_mode(ptr, name, pass, unlen, uplen);
	}
	return -1;
}

