/*
 * items_info_db.c
 *
 *  Created on: Jun 13, 2020
 *      Author: khubaibumer
 */

#include <thor.h>
#define ITEM_TABLE "ItemsInformation"
#define SCHEMA " (Name,Quantity,Price,AdditionalInfo) "

int __add_items(void *ptr, const char *name, const char *quantity,
		const char *price, const char *extra) {

	char sql[256] = "INSERT INTO " ITEM_TABLE SCHEMA "VALUES( ";
	size_t len = strlen(sql);
	sprintf(&sql[len], "'%s','%s','%s','%s'); ", name, quantity, price, extra);
	log.i("Query is: %s\n", sql);

	int rt = sqlite3_exec(CAST(ptr)->db.db_hndl, sql, NULL, 0,
			&CAST(ptr)->rpc.return_value.response);
	if (rt != SQLITE_OK) {
		log.e("Error: %s\n", CAST(ptr)->rpc.return_value.response);
		return -1;
	}

	return 0;
}

FILE *fp1 = NULL;

int get_items_list(void *_buf, int argc, char **argv, char **azColName) {
	int i;

	char buf[5 * 1000] = { };
	size_t len = 0;
	len += sprintf(&(buf)[len], "%s", "{ ");
	for (i = 0; i < argc; i++) {
		len += sprintf(&(buf)[len], "\"%s\" : \"%s\",", azColName[i],
				argv[i] ? argv[i] : "NULL");
	}
	(buf)[--len] = '\0';
	len += sprintf(&buf[len], "}%s", ",");

	fprintf(fp1, "%s", buf);
	return 0;
}

int __get_all_items(void *ptr) {

	static char stbuf[5 * 1000 * 1000] = { }; // 5mb
	fp1 = fopen("/dev/null", "w+");
	setvbuf(fp1, stbuf, _IOFBF, BUFSIZ);

	char sql[] = "select * from " ITEM_TABLE ";";
	log.i("Query is: %s\n", sql);

	CAST(ptr)->rpc.return_value.ret.value = calloc(5 * 1000 * 1000, sizeof(char));
	CAST(ptr)->rpc.return_value.ret.len = sprintf(
	CAST(ptr)->rpc.return_value.ret.value, "%s", "[ ");
	int rt = sqlite3_exec(CAST(ptr)->db.db_hndl, sql, get_items_list,
			&CAST(ptr)->rpc.return_value.ret,
			&CAST(ptr)->rpc.return_value.response);
	if (rt != SQLITE_OK) {
		log.e("Error: %s\n", CAST(ptr)->rpc.return_value.response);
		return -1;
	}

	sprintf(CAST(ptr)->rpc.return_value.ret.value, "[ %s", stbuf);

	CAST(ptr)->rpc.return_value.ret.len = strlen(
			CAST(ptr)->rpc.return_value.ret.value);
	CAST(ptr)->rpc.return_value.ret.value[CAST(ptr)->rpc.return_value.ret.len
			- 1] = ']';
	CAST(ptr)->rpc.return_value.ret.value[CAST(ptr)->rpc.return_value.ret.len] =
			'\0';

	memset(stbuf, 0, strlen(stbuf) + 1);
	log.v("%s\n", CAST(ptr)->rpc.return_value.ret.value);

	fclose(fp1);
	fp1 = NULL;

	return 0;
}
