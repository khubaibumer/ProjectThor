/*
 * main.c
 *
 *  Created on: May 27, 2020
 *      Author: khubaibumer
 */

#include <stdio.h>
#include <stdlib.h>

#include <thor.h>

extern int register_users();

int main(int argc, char **argv) {

	CAST(THIS)->db.init_db(THIS);
	register_users();
	CAST(THIS)->db.psswd_db.creat_usr(THIS, "admin", "admin", ROOT_USR);
	CAST(THIS)->db.psswd_db.update_usr(THIS, NULL, NULL, NULL, NULL);
	CAST(THIS)->db.psswd_db.dlt_usr(THIS, NULL, NULL);

	CAST(THIS)->server.up(THIS);

//	char *hash = NULL;
//	CAST(THIS)->ssl_tls.hash(THIS, "Hello WORlD!\n", &hash);

	CAST(THIS)->server.accept(THIS);
//	CAST(THIS)->server.kick(THIS, 12);
	CAST(THIS)->server.list(THIS);
	CAST(THIS)->server.down(THIS);

	return 0;
}

