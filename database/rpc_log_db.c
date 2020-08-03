/*
 * rpc_log_db.c
 *
 *  Created on: Jun 13, 2020
 *      Author: khubaibumer
 */

#include <rpc.h>

#define LOGS_TABLE "TransactionLogs"
#define SCHEMA " (User,UserId,Epoch,Process,LogType,LatLong,TxId,StackTrace) "

int __log_rpc_command(void *ptr, int type, const char *process,
		const char *command, char *latlong, char *tx_id) {

	time_t now = time(NULL);

	char sql[8000] = "INSERT INTO " LOGS_TABLE SCHEMA "VALUES( ";
	size_t len = strnlen(sql, 8000);
	if (type == REQD) {
		sprintf(&sql[len], "'%s:%d','%d',%d,'%s','%s','%s','%s','%s'); ",
				CAST(ptr)->client.ip,
				CAST(ptr)->client.port, CAST(ptr)->user.uid, now, process,
				"Requested", latlong, tx_id, command);
	} else {
		sprintf(&sql[len], "'%s:%d','%d',%d,'%s','%s','%s','%s','%s'); ",
				CAST(ptr)->client.ip,
				CAST(ptr)->client.port, CAST(ptr)->user.uid, now, process,
				"AUTO", latlong, tx_id, command);
	}
	log.i("Query is: %s\n", sql);

	int rt = sqlite3_exec(CAST(ptr)->db.db_hndl, sql, NULL, 0,
			&CAST(ptr)->rpc.return_value.response);
	if (rt != SQLITE_OK) {
		log.e("Error: %s\n", CAST(ptr)->rpc.return_value.response);
		return -1;
	}

	return 0;
}

