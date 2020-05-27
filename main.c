/*
 * main.c
 *
 *  Created on: May 27, 2020
 *      Author: khubaibumer
 */

#include <stdio.h>
#include <stdlib.h>

#include <thor.h>

int main(int argc, char **argv) {

	DECLARE_SYMBOL(char *, str) = "Hello World!\n";

	printf("%s", str);

	CAST(THIS)->server.accept();
	CAST(THIS)->server.down();
	CAST(THIS)->server.kick(12);
	CAST(THIS)->server.list();
	CAST(THIS)->server.listen();
	CAST(THIS)->server.up();

	return 0;
}

