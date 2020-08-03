/*
 * items_images.c
 *
 *  Created on: Jul 17, 2020
 *      Author: khubaibumer
 */

#include <thor.h>
#include <mappings.h>

#define IMAGE_TABLE "ItemImages"
#define SCHEMA " (UPC,Image,Extra) "

int __update_item_image(void *ptr, const char *qual, int key,
		const char *updated) {

	const char *col_name = get_mapping(key);
	if (!col_name)
		return -1;

	char *sql = calloc(MB(12), sizeof(char));

	sprintf(sql,
			"UPDATE %s SET %s='%s' where ID=(SELECT ID FROM %s where UPC='%s' );",
			IMAGE_TABLE, col_name, updated, IMAGE_TABLE, qual);

	log.i("Query is: %s\n", sql);

	int rt = sqlite3_exec(CAST(ptr)->db.db_hndl, sql, NULL, 0,
			&CAST(ptr)->rpc.return_value.response);
	if (rt != SQLITE_OK) {
		log.e("Error: %s\n", CAST(ptr)->rpc.return_value.response);
		free(sql);
		return -1;
	}

	free(sql);
	return 0;
}

int __add_image(void *ptr, const char *upc, const char *image) {

	char *sql = calloc(MB(12), sizeof(char));

	sprintf(sql,
			"INSERT INTO " IMAGE_TABLE SCHEMA "VALUES( " "'%s','%s',' '); ",
			upc, image);
	log.i("Query is: %s\n", sql);

	int rt = sqlite3_exec(CAST(ptr)->db.db_hndl, sql, NULL, 0,
			&CAST(ptr)->rpc.return_value.response);
	if (rt != SQLITE_OK) {
		log.e("Error: %s\n", CAST(ptr)->rpc.return_value.response);
		free(sql);
		return -1;
	}

	free(sql);
	return 0;
}

int __delete_image(void *ptr, const char *upc) {

	char sql[256] = "DELETE FROM " IMAGE_TABLE " where UPC= ";
	size_t len = strnlen(sql, 210); // Max UPC is 13 character long
	sprintf(&sql[len], "'%s'; ", upc);
	log.i("Query is: %s\n", sql);

	int rt = sqlite3_exec(CAST(ptr)->db.db_hndl, sql, NULL, 0,
			&CAST(ptr)->rpc.return_value.response);
	if (rt != SQLITE_OK) {
		log.e("Error: %s\n", CAST(ptr)->rpc.return_value.response);
		return -1;
	}

	return 0;
}

FILE *img;

int get_upc_list(void *_buf, int argc, char **argv, char **azColName) {
	int i;

	char *buf = calloc(MB(2), sizeof(char));
	size_t len = 0;
	len += sprintf(&(buf)[len], "%s", "{ ");
	for (i = 0; i < argc; i++) {
		len += sprintf(&(buf)[len], "\"%s\" : \"%s\",", azColName[i],
				argv[i] ? argv[i] : "NULL");
	}
	(buf)[--len] = '\0';
	len += sprintf(&buf[len], "}%s", ",");

	assert(len > 0);

	fprintf(img, "%s", buf);

	free(buf);
	return 0;
}

int __get_all_image_upc(void *ptr) {
	char *stbuf = calloc(MB(14), sizeof(char));
	img = fopen("/dev/null", "w+");
	setvbuf(img, stbuf, _IOFBF, MB(3));

	char sql[] = "SELECT UPC FROM " IMAGE_TABLE " ;";
	log.i("Query is:%s\n", sql);

	CAST(ptr)->rpc.return_value.ret.value = calloc(MB(3), sizeof(char));
	int rt = sqlite3_exec(CAST(ptr)->db.db_hndl, sql, get_upc_list,
			&CAST(ptr)->rpc.return_value.ret,
			&CAST(ptr)->rpc.return_value.response);
	if (rt != SQLITE_OK) {
		free(stbuf);
		fclose(img);
		img = NULL;
		log.e("Error: %s\n", CAST(ptr)->rpc.return_value.response);
		return -1;
	}

	sprintf(CAST(ptr)->rpc.return_value.ret.value, "[ %s", stbuf);

	CAST(ptr)->rpc.return_value.ret.len = strnlen(
			CAST(ptr)->rpc.return_value.ret.value, MB(3));
	CAST(ptr)->rpc.return_value.ret.value[CAST(ptr)->rpc.return_value.ret.len
			- 1] = ']';
	CAST(ptr)->rpc.return_value.ret.value[CAST(ptr)->rpc.return_value.ret.len] =
			'\0';

	log.v("%s\n", CAST(ptr)->rpc.return_value.ret.value);

	fclose(img);
	free(stbuf);
	img = NULL;

	return 0;
}

int get_image_from_upc(void *_buf, int argc, char **argv, char **azColName) {
	int i;

	char *buf = calloc(MB(12), sizeof(char));
	size_t len = 0;
	len += sprintf(&(buf)[len], "%s", "{ ");
	for (i = 0; i < argc; i++) {
		len += sprintf(&(buf)[len], "\"%s\" : \"%s\",", azColName[i],
				argv[i] ? argv[i] : "NULL");
	}
	(buf)[--len] = '\0';
	len += sprintf(&buf[len], "}%s", ",");

	assert(len > 0);

	fprintf(img, "%s", buf);

	free(buf);
	return 0;
}

int __get_image_for_upc(void *ptr, const char *upc) {

	char *stbuf = calloc(MB(14), sizeof(char));
	img = fopen("/dev/null", "w+");
	setvbuf(img, stbuf, _IOFBF, MB(14));

	char sql[1024] = { };
	sprintf(sql, "SELECT IMAGE FROM " IMAGE_TABLE " WHERE UPC='%s' ;", upc);
	log.i("Query is:%s\n", sql);

	CAST(ptr)->rpc.return_value.ret.value = calloc(MB(15), sizeof(char));
	int rt = sqlite3_exec(CAST(ptr)->db.db_hndl, sql, get_image_from_upc,
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

	log.v("%s\n", CAST(ptr)->rpc.return_value.ret.value);

	fclose(img);
	free(stbuf);
	img = NULL;

	return 0;
}
