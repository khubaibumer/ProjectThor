/*
 * db_manager.c
 *
 *  Created on: Jun 12, 2020
 *      Author: khubaibumer
 */

#include <rpc.h>

int __process_db_user_cmds(void *node, enum commands action) {

	switch (action) {
	case add: {
		char *name = strtok(NULL, ",");
		char *pass = strtok(NULL, ",");
		int mode = atoi(strtok(NULL, ","));
		if (GETTHOR(node)->db.psswd_db.creat_usr) {
			if (GETTHOR(node)->db.psswd_db.creat_usr(GETTHOR(node), name, pass,
					mode) == 0)
				send_response(node, "resp,ok,0,response,%s\n",
				GETTHOR(node)->rpc.return_value.response == NULL ? "success" :
				GETTHOR(node)->rpc.return_value.response);
			else
				send_response(node, "resp,fail,reason,%s\n",
				GETTHOR(node)->rpc.return_value.response == NULL ? "db error" :
				GETTHOR(node)->rpc.return_value.response);
		} else {
			send_response(node, "%s\n", "resp,fail,reason,unauthorized");
		}
	}
		break;
	case del: {
		char *name = strtok(NULL, ",");
		char *pass = strtok(NULL, ",");
		int mode = atoi(strtok(NULL, ","));
		if (GETTHOR(node)->db.psswd_db.dlt_usr) {
			if (GETTHOR(node)->db.psswd_db.dlt_usr(GETTHOR(node), name, pass)
					== 0)
				send_response(node, "resp,ok,0,response,%s\n",
				GETTHOR(node)->rpc.return_value.response == NULL ? "success" :
				GETTHOR(node)->rpc.return_value.response);
			else
				send_response(node, "resp,fail,reason,%s\n",
				GETTHOR(node)->rpc.return_value.response == NULL ? "db error" :
				GETTHOR(node)->rpc.return_value.response);
		} else {
			send_response(node, "%s\n", "resp,fail,reason,unauthorized");
		}
	}
		break;
	case update: {
	}
		break;
	};

	return 0;
}

PRIVATE int __process_db_cmds(void *node) {

	switch (find_cmd(strtok(NULL, ","))) {
	case add: {
		int table = find_cmd(strtok(NULL, ","));
		switch (table) {
		case user:
			__process_db_user_cmds(node, add);
			break;
		default:
			// error Response
			send_response(node, "%s\n", "resp,fail,reason,invalid command");
			break;
		};
	}
		break;
	case del: {
		int table = find_cmd(strtok(NULL, ","));
		switch (table) {
		case user:
			__process_db_user_cmds(node, del);
			break;
		default:
			// error Response
			send_response(node, "%s\n", "resp,fail,reason,invalid command");
			break;
		};
	}
		break;
	case update: {
		int table = find_cmd(strtok(NULL, ","));
		switch (table) {
		case user:
			//TODO: implement
			break;
		default:
			//error Respose
			send_response(node, "%s\n", "resp,fail,reason,invalid command");
			break;
		};
	}
		break;

	default:
		// error Response
		send_response(node, "%s\n", "resp,fail,reason,invalid command");
		break;
	};

	if (GETTHOR(node)->rpc.return_value.response)
		sqlite3_free(GETTHOR(node)->rpc.return_value.response);

	return 0;
}
