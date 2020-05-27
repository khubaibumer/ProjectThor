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
		{ "audit", "audit", sizeof("audit"), ELVT_USR },
};

DECLARE_SYMBOL(const size_t, usr_list_len) = sizeof(valid_users)
		/ sizeof(valid_users_t);

int get_user_mode(const char *id, const char *pas, const size_t ilen,
		const size_t _plen) {

	for (int i = 0; i < usr_list_len; i++) {
		if ((ilen == valid_users[i].plen) && _plen == valid_users[i].plen) {
			if ((memcmp(id, valid_users[i].name, ilen) == 0)
					&& (memcmp(pas, valid_users[i].pass, ilen) == 0)) {
				return valid_users[i].mode;
			}
		}
	}
	return -1;
}

int __auth(int fd) {

	char name[1024] = { };
	char pass[1024] = { };
	int wb = send(fd, "Enter Username: ", 1024, 0);
	wb = recv(fd, name, 1024, 0);
	wb = send(fd, "Enter Password: ", 1024, 0);
	wb = recv(fd, pass, 1024, 0);

	if (wb < 0)
		perror("something wrong!");

	size_t unlen = strlen(name);
	size_t uplen = strlen(pass);

	return get_user_mode(name, pass, unlen, uplen);
}

