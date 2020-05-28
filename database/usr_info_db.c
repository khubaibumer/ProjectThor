/*
 * usr_info_db.c
 *
 *  Created on: May 28, 2020
 *      Author: khubaibumer
 */

#include <thor.h>

#include <sqlite3.h>

int callback_(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

int __create_usr_table(void *ptr, const char *uname, const char *upsswd, int role) {

	printf("%s() called\n", __func__);

	char *err_msg = NULL;
	char *psswd = NULL;
	CAST(ptr)->ssl_tls.hash(ptr, upsswd, &psswd);

	char sql[256] =
			"INSERT INTO UserPsswd(UserName,UserPsswd,PrivMode,AdditionalInfo) "
					"VALUES( ";
	size_t len = strlen(sql);
	sprintf(&sql[len], "'%s','%s',%d,''); " , uname, psswd, role);

	printf("Query is: %s\n", sql);

	int rt = sqlite3_exec(CAST(ptr)->db.db_hndl, sql, callback_, 0, &err_msg);
	if(rt != SQLITE_OK) {
		printf("Error: %s\n", err_msg);
		sqlite3_free(err_msg);
	}

	free(psswd);

	return 0;
}

int __update_usr_table(void *ptr, const char *oname, const char *opsswd,
		const char *nname, const char *npsswd) {

	printf("%s() called\n", __func__);

	return 0;
}

int __dlt_usr_info(void *ptr, const char *uname, const char *upsswd) {

	printf("%s() called\n", __func__);

	return 0;
}
