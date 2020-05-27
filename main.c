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

	CAST(THIS)->server.up(THIS);
	CAST(THIS)->server.accept(THIS);
//	CAST(THIS)->server.kick(THIS, 12);
	CAST(THIS)->server.list(THIS);
	CAST(THIS)->server.down(THIS);

	return 0;
}

