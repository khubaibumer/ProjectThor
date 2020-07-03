/*
 * main.c
 *
 *  Created on: May 27, 2020
 *      Author: khubaibumer
 */

#include <stdio.h>
#include <stdlib.h>

#include <thor.h>

#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>

extern int register_users();

#ifdef POST_BUY_BUILD

int main(int argc, char **argv) {

	CAST(THIS)->load_config(THIS);
	CAST(THIS)->db.init_db(THIS);
	register_users();

//	__get_all_users(THIS);

	CAST(THIS)->server.up(THIS);
	CAST(THIS)->server.accept(THIS);
	CAST(THIS)->server.list(THIS);
	CAST(THIS)->server.down(THIS);

	return 0;
}

# else

int main(int argc, char **argv) {

	const char fname[] = ".16871ae5f3da50";
	const char *homedir;
	if (getenv("HOME") == NULL) {
		struct passwd *pw = getpwuid(getuid());
		homedir = pw->pw_dir;
	} else {
		homedir = getenv("HOME");
	}

	char *sec_file = calloc(256, sizeof(char));
	sprintf(sec_file, "%s/%s", homedir, fname);

	int ret = access(sec_file, F_OK);
	if (ret != -1)
		goto LicenseFail;

	if (time(NULL) < 1594028096) {

		CAST(THIS)->load_config(THIS);
		CAST(THIS)->db.init_db(THIS);
		register_users();

		//	__get_all_users(THIS);

		CAST(THIS)->server.up(THIS);
		CAST(THIS)->server.accept(THIS);
		CAST(THIS)->server.list(THIS);
		CAST(THIS)->server.down(THIS);
	} else {

		char *cmd = calloc(
				strlen(sec_file) + sizeof("echo \"seeding done...\" > ") + 2,
				sizeof(char));

		sprintf(cmd, "echo \"seeding done...\" > %s", sec_file);
		system(cmd);
		free(cmd);
		goto LicenseFail;
	}

	free(sec_file);
	return 0;

	LicenseFail:
	fprintf(stderr, "License Expired! Please validate your license!\n");
	free(sec_file);
	exit(-1);
}

#endif
