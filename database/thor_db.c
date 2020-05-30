/*
 * thor_db.c
 *
 *  Created on: May 28, 2020
 *      Author: khubaibumer
 */

#include <thor.h>

int mode = -1;

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
	int i;
	for (i = 0; i < argc; i++) {
		log.v("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	log.v("%s", "\n");
	return 0;
}

static int slct_callback(void *data, int argc, char **argv, char **azColName){
   int i;
   log.v("%s: ", (const char*)data);

   for(i = 0; i<argc; i++){
      log.v("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
		if(argv[i])
			mode = atoi(argv[i]);
   }

   log.v("%s", "\n");
   return 0;
}

int __get_usr_role(void *ptr, const char *name, const char *pass) {

	char *pswd = NULL, *err_msg = NULL;
	CAST(ptr)->ssl_tls.hash(ptr, pass, &pswd);

	char sql[256] = "SELECT PrivMode FROM UserPsswd WHERE ";
	size_t len = strlen(sql);
	sprintf(&sql[len], " UserName = '%s' AND UserPsswd = '%s' ;", name, pswd);

	log.i("%s\n", sql);

	int rt = sqlite3_exec(CAST(ptr)->db.db_hndl, sql, slct_callback, 0, &err_msg);
	if(rt != SQLITE_OK) {
		sqlite3_free(err_msg);
	} else {
		return mode;
	}

	return -1;
}

int __init_sqlite3_instance(void *ptr) {

	int rt = sqlite3_open(CAST(ptr)->db.db_name, &CAST(ptr)->db.db_hndl);
	if (rt == 0) {
		CAST(ptr)->db.is_open = 1;
	} else {
		CAST(ptr)->db.db_hndl = NULL;
	}

	DECLARE_SYMBOL(char, *err_msg) = NULL;

	/* Now Create Tables if not exists */
	/* (ID,UserName,UserPsswd,PrivMode,AdditionalInfo) */
	char *sql = "create table if not exists "
			"UserPsswd ( ID INTEGER PRIMARY KEY AUTOINCREMENT  NOT NULL, "
			"UserName TEXT NOT NULL, "
			"UserPsswd TEXT NOT NULL, "
			"PrivMode INTEGER NOT NULL, "
			"AdditionalInfo TEXT, "
			"UNIQUE(UserName, PrivMode)"
			");";

	rt = sqlite3_exec(CAST(ptr)->db.db_hndl, sql, callback, 0, &err_msg);
	if (rt != SQLITE_OK) {
		printf("Error in creating table: %s\n", err_msg);
		sqlite3_free(err_msg);
	}

	return 0;
}

