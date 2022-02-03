/*
 * usr_info_db.c
 *
 *  Created on: May 28, 2020
 *      Author: khubaibumer
 */

#include <thor.h>
#include <sqlite3.h>
#include <mappings.h>

#define USER_TABLE " UserInformation "
#define SCHEMA " (UserName,UserPsswd,PrivMode,IsLogged,AdditionalInfo) "

int __update_user_info(void *ptr, const char *qual, int key,
		const char *updated) {

	/*	update UserPsswd set UserName='admin' where ID=(select ID from UserPsswd where UserName='Admin');	*/

	char sql[256] = { };
	char *hashed = NULL;
	if (key == userpass) {
		// We need to hash password
		CAST(ptr)->ssl_tls.hash(ptr, updated, &hashed);
	} else {
		hashed = (char*) updated;
	}

	const char *col_name = get_mapping(key);
	if (!col_name)
		return -1;

	sprintf(sql,
			"UPDATE %s SET %s='%s' where ID=(SELECT ID FROM %s where UserName='%s' );",
			USER_TABLE, col_name, hashed, USER_TABLE, qual);

	log.i("Query is: %s\n", sql);

	int rt = sqlite3_exec(CAST(ptr)->db.db_hndl, sql, NULL, 0,
			&CAST(ptr)->rpc.return_value.response);
	if (rt != SQLITE_OK) {
		log.e("Error: %s\n", CAST(ptr)->rpc.return_value.response);

		if (key == userpass)
			free(hashed);
		return -1;
	}

	return 0;
}

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

	char sql[256] = "INSERT INTO " USER_TABLE SCHEMA "VALUES( ";
	size_t len = strnlen(sql, 256);
	sprintf(&sql[len], "'%s','%s',%d,%d,''); ", uname, psswd, role, 1); // by default every user is logged

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

int __dlt_usr_info(void *ptr, const char *uname, const char *upsswd) {

	log.d("%s() called\n", __func__);

	return 0;
}

FILE *fp = NULL;

int get_user_list(void *_buf, int argc, char **argv, char **azColName) {
	int i;

	char buf[10000] = { };
	size_t len = 0;
	len += sprintf(&(buf)[len], "%s", "{ ");
	for (i = 0; i < argc; i++) {
		len += sprintf(&(buf)[len], "\"%s\" : \"%s\",", azColName[i],
				argv[i] ? argv[i] : "NULL");
	}
	(buf)[--len] = '\0';
	len += sprintf(&buf[len], "}%s", ",");

	assert(len > 0);

	fprintf(fp, "%s", buf);
	return 0;
}

int __get_all_users(void *ptr) {

	char *stbuf = calloc(MB(2), sizeof(char));
	fp = fopen("/dev/null", "w+");
	setvbuf(fp, stbuf, _IOFBF, BUFSIZ);

	char sql[] = "select * from " USER_TABLE ";";
	log.i("Query is: %s\n", sql);

	CAST(ptr)->rpc.return_value.ret.value = calloc(MB(1), sizeof(char));
	CAST(ptr)->rpc.return_value.ret.len = sprintf(
	CAST(ptr)->rpc.return_value.ret.value, "%s", "[ ");
	int rt = sqlite3_exec(CAST(ptr)->db.db_hndl, sql, get_user_list,
			&CAST(ptr)->rpc.return_value.ret,
			&CAST(ptr)->rpc.return_value.response);
	if (rt != SQLITE_OK) {
		free(stbuf);
		fclose(fp);
		log.e("Error: %s\n", CAST(ptr)->rpc.return_value.response);
		return -1;
	}

	sprintf(CAST(ptr)->rpc.return_value.ret.value, "[ %s", stbuf);

	CAST(ptr)->rpc.return_value.ret.len = strnlen(
			CAST(ptr)->rpc.return_value.ret.value, MB(1));
	CAST(ptr)->rpc.return_value.ret.value[CAST(ptr)->rpc.return_value.ret.len
			- 1] = ']';
	CAST(ptr)->rpc.return_value.ret.value[CAST(ptr)->rpc.return_value.ret.len] =
			'\0';

	log.v("%s\n", CAST(ptr)->rpc.return_value.ret.value);

	fclose(fp);
	fp = NULL;
	free(stbuf);

	return 0;
}
