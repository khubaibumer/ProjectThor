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

static const valid_users_t valid_users[] = { { "admin", "admin",
		sizeof("admin"), ROOT_USR },
		{ "user", "user", sizeof("user"), DFL_USR }, { "audit", "audit",
				sizeof("audit"), ELVT_USR }, };

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

int __auth(void *ptr, int fd) {

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

	if(name[unlen-1] == '\n')
		name[--unlen] = '\0';

	if(pass[uplen-1] == '\n')
		pass[--uplen] = '\0';

	return get_user_mode(ptr, name, pass, unlen, uplen);
}

