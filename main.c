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

//	__get_all_users(THIS);

	CAST(THIS)->server.up(THIS);
	CAST(THIS)->server.accept(THIS);
	CAST(THIS)->server.list(THIS);
	CAST(THIS)->server.down(THIS);

	return 0;
}

