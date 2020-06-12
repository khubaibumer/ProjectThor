/*
 * usr_info_db.c
 *
 *  Created on: May 28, 2020
 *      Author: khubaibumer
 */

#include <thor.h>
#include <sqlite3.h>

#define USER_TABLE "UserInformation"

int callback_(void *NotUsed, int argc, char **argv, char **azColName) {
	int i;
	for (i = 0; i < argc; i++) {
		log.v("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	log.v("%s", "\n");
	return 0;
}

int __create_usr_table(void *ptr, const char *uname, const char *upsswd,
		int role) {

	char *psswd = NULL;

	CAST(ptr)->ssl_tls.hash(ptr, upsswd, &psswd);

	char sql[256] =
			"INSERT INTO " USER_TABLE "(UserName,UserPsswd,PrivMode,AdditionalInfo) "
					"VALUES( ";
	size_t len = strlen(sql);
	sprintf(&sql[len], "'%s','%s',%d,''); ", uname, psswd, role);

	log.i("Query is: %s\n", sql);

	int rt = sqlite3_exec(CAST(ptr)->db.db_hndl, sql, NULL, 0,
			&CAST(ptr)->rpc.return_value.response);
	if (rt != SQLITE_OK) {
		log.e("Error: %s\n", CAST(ptr)->rpc.return_value.response);
		free(psswd);
		return -1;
	}

	free(psswd);

	return 0;
}

int __update_usr_table(void *ptr, const char *oname, const char *opsswd,
		const char *nname, const char *npsswd) {

	log.d("%s() called\n", __func__);
	//update UserPsswd set UserName='admin' where ID=(select ID from UserPsswd where UserName='Admin');

	return 0;
}

int __dlt_usr_info(void *ptr, const char *uname, const char *upsswd) {

	log.d("%s() called\n", __func__);

	return 0;
}

int get_user_list (void *NotUsed, int argc, char **argv, char **azColName) {
	int i;
	printf("%s", "{ ");
	for (i = 0; i < argc; i++) {
		printf("\"%s\" : \"%s\",", azColName[i], argv[i] ? argv[i] : "NULL");
//		log.v("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("}%s", ",");
	return 0;
}

int __get_all_users(void *ptr) {


	char sql[] = "select * from " USER_TABLE ";";
	log.i("Query is: %s\n", sql);

	printf("%s", "[ ");
	int rt = sqlite3_exec(CAST(ptr)->db.db_hndl, sql, get_user_list, 0,
			&CAST(ptr)->rpc.return_value.response);
	if (rt != SQLITE_OK) {
		log.e("Error: %s\n", CAST(ptr)->rpc.return_value.response);
		return -1;
	}
	printf("%s\n", "] ");

	return 0;
}


