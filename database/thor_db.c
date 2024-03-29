/*
 * thor_db.c
 *
 *  Created on: May 28, 2020
 *      Author: khubaibumer
 */

#include <thor.h>

#define USER_TABLE "UserInformation"
#define ITEM_TABLE "ItemsInformation"
#define LOGS_TABLE "TransactionLogs"
#define IMAGES_TABLE "ItemImages"

PRIVATE
static int get_user_logging_mode_from_db(void *data, int argc, char **argv,
		char **azColName) {
	int *_bit = (int*) data;
	for (int i = 0; i < argc; i++) {
		log.v("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
		if (argv[i])
			*_bit = atoi(argv[i]);
	}
	return 0;
}

uint8_t __is_logged(void *ptr, const char *name) {

	char sql[128] = {};
	int mode = 1;
	char *err_msg = NULL;

	sprintf(sql, "SELECT IsLogged from %s where UserName='%s' ", USER_TABLE, name);

	int rt = sqlite3_exec(CAST(ptr)->db.db_hndl, sql, get_user_logging_mode_from_db,
			&mode, &err_msg);
	if (rt != SQLITE_OK) {
		sqlite3_free(err_msg);
	} else {
		return mode;
	}

	return 1; // Default case is every user rpc-cmd is logged
}


static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
	int i;
	for (i = 0; i < argc; i++) {
		log.v("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	log.v("%s", "\n");

	return 0;
}

PRIVATE
static int get_user_mode_from_db(void *data, int argc, char **argv,
		char **azColName) {
	int *_mode = (int*) data;
	for (int i = 0; i < argc; i++) {
		log.v("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
		if (argv[i])
			*_mode = atoi(argv[i]);
	}

	return 0;
}

PRIVATE
int __get_usr_role(void *ptr, const char *name, const char *pass) {

	char *pswd = NULL, *err_msg = NULL;
	int mode = -1;
	CAST(ptr)->ssl_tls.hash(ptr, pass, &pswd);

	char sql[256] = "SELECT PrivMode FROM " USER_TABLE " WHERE ";
	size_t len = strnlen(sql, 256);
	sprintf(&sql[len], " UserName = '%s' AND UserPsswd = '%s' ;", name, pswd);

	log.i("%s\n", sql);
	free(pswd);

	int rt = sqlite3_exec(CAST(ptr)->db.db_hndl, sql, get_user_mode_from_db,
			&mode, &err_msg);
	if (rt != SQLITE_OK) {
		sqlite3_free(err_msg);
	} else {
		return mode;
	}

	return -1;
}

PRIVATE
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
	USER_TABLE " ( ID INTEGER PRIMARY KEY AUTOINCREMENT  NOT NULL, "
	"UserName TEXT NOT NULL, "
	"UserPsswd TEXT NOT NULL, "
	"PrivMode INTEGER NOT NULL, "
	"IsLogged INTEGER NOT NULL, "
	"AdditionalInfo TEXT, "
	"UNIQUE(UserName, PrivMode)"
	");";

	rt = sqlite3_exec(CAST(ptr)->db.db_hndl, sql, callback, 0, &err_msg);
	if (rt != SQLITE_OK) {
		printf("Error in creating table: %s\n", err_msg);
		sqlite3_free(err_msg);
	}

	/*	Create Items table	*/
	err_msg = NULL;
	/* Now Create Tables if not exists */
	/* (ID,Name,Quantity,Price,AdditionalInfo) */
	char *sql1 = "create table if not exists "
	ITEM_TABLE " ( ID INTEGER PRIMARY KEY AUTOINCREMENT  NOT NULL, "
	"UPC TEXT NOT NULL, "
	"Quantity TEXT NOT NULL, "
	"Price TEXT NOT NULL, "
	"Name TEXT, "
	"UNIQUE(Name)"
	");";

	rt = sqlite3_exec(CAST(ptr)->db.db_hndl, sql1, callback, 0, &err_msg);
	if (rt != SQLITE_OK) {
		printf("Error in creating table: %s\n", err_msg);
		sqlite3_free(err_msg);
	}

	/*	Create Transaction Logs	*/
	err_msg = NULL;
	/* Now Create Tables if not exists */
	/* (ID,Name,Quantity,Price,AdditionalInfo) */
	char *sql2 = "create table if not exists "
	LOGS_TABLE " ( ID INTEGER PRIMARY KEY AUTOINCREMENT  NOT NULL, "
	"User TEXT NOT NULL, "
	"UserId TEXT NOT NULL, "
	"Epoch TEXT NOT NULL, "
	"Process TEXT NOT NULL, "
	"LogType TEXT NOT NULL,"
	"LatLong TEXT NOT NULL,"
	"TxId TEXT NOT NULL,"
	"StackTrace TEXT NOT NULL "
	");";

	rt = sqlite3_exec(CAST(ptr)->db.db_hndl, sql2, callback, 0, &err_msg);
	if (rt != SQLITE_OK) {
		printf("Error in creating table: %s\n", err_msg);
		sqlite3_free(err_msg);
	}

	/*	Create Item Images Table	*/
	err_msg = NULL;
	/* Now Create Tables if not exists */
	/* (ID,UPC,Image,ExtraInfo) */
	char *sql3 = "create table if not exists "
	IMAGES_TABLE " ( ID INTEGER PRIMARY KEY AUTOINCREMENT  NOT NULL, "
	"UPC TEXT NOT NULL, "
	"Image BLOB NOT NULL, "
	"Extra TEXT, "
	"UNIQUE(UPC)"
	");";

	rt = sqlite3_exec(CAST(ptr)->db.db_hndl, sql3, callback, 0, &err_msg);
	if (rt != SQLITE_OK) {
		printf("Error in creating table: %s\n", err_msg);
		sqlite3_free(err_msg);
	}

	return 0;
}

