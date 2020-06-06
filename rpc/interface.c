/*
 * interface.c
 *
 *  Created on: Jun 5, 2020
 *      Author: khubaibumer
 */

#include <thor.h>

enum commands {
	auth,
	db,
	user,
	add,
	del,
	update,
	userid,
	userpass,
	usermode,
};

typedef struct {
	const char *key;
	int value;
} rpc_commands_t;

const static rpc_commands_t rpc_commands[] = {
		{ "auth", auth },
		{ "db", db },
		{ "user", user },
		{ "add", add },
		{ "del", del },
		{ "update", update },
		{ "userid", userid },
		{ "userpass", userpass },
		{ "usermode", usermode },
};

static inline int find_cmd(char *in) {
	if(in == NULL)
		return -1;

	for(int i = 0; i < sizeof(rpc_commands)/sizeof(rpc_commands_t); i++) {
		if(strstr(in, rpc_commands[i].key)) {
			return i;
		}
	}
	return -1;
}

int send_response(void *node, const char *fmt, ...) {

	char repose[64] = { };
	va_list arg;
	va_start(arg, fmt);
	int size = vsprintf(repose, fmt, arg);
	va_end(arg);

	return GETTHOR(node)->ssl_tls.write(GETTHOR(node), repose, size+1);
}

void __process_cmd(void *node, char *cmd) {

	if(strstr(cmd, "bye")) {
		// We need to close client connection respectfuly
		GETTHOR(node)->ssl_tls.close(GETTHOR(node));
		CAST(THIS)->free(node);

		return ; // Done and done
	}

	/*	Other than bye are valid rpc_calls	*/
	if(strlen(cmd) > 1 && strstr(cmd, ",")) {
		switch(find_cmd(strtok(cmd, ","))) {
		case auth:
			send_response(node, "auth,ok,0");
			break; // Nothing to do
		case db: {
			switch(find_cmd(strtok(NULL, ","))) {
			case add: {
				int table = find_cmd(strtok(NULL, ","));
				if(table == user) {
					char *name = strtok(NULL, ",");
					char *pass = strtok(NULL, ",");
					int mode = atoi(strtok(NULL, ","));
					if(GETTHOR(node)->db.psswd_db.creat_usr) {
						if(GETTHOR(node)->db.psswd_db.creat_usr(GETTHOR(node), name, pass, mode) == 0)
							send_response(node, "resp,ok,0,response,%s\n", GETTHOR(node)->rpc.return_value.response == NULL ? "success" : GETTHOR(node)->rpc.return_value.response);
						else
							send_response(node, "resp,fail,reason,%s\n", GETTHOR(node)->rpc.return_value.response == NULL ? "db error" : GETTHOR(node)->rpc.return_value.response);
					} else {
						send_response(node, "%s\n", "resp,fail,reason,unauthorized");
					}
				}
				if(GETTHOR(node)->rpc.return_value.response)
					sqlite3_free(GETTHOR(node)->rpc.return_value.response);
			}
				break;
			case del: {
				int table = find_cmd(strtok(NULL, ","));
				if(table == user) {
					char *name = strtok(NULL, ",");
					char *pass = strtok(NULL, ",");
					int mode = atoi(strtok(NULL, ","));
					if(GETTHOR(node)->db.psswd_db.dlt_usr) {
						if(GETTHOR(node)->db.psswd_db.dlt_usr(GETTHOR(node), name, pass) == 0)
							send_response(node, "resp,ok,0,response,%s\n", GETTHOR(node)->rpc.return_value.response == NULL ? "success" : GETTHOR(node)->rpc.return_value.response);
						else
							send_response(node, "resp,fail,reason,%s\n", GETTHOR(node)->rpc.return_value.response == NULL ? "db error" : GETTHOR(node)->rpc.return_value.response);
					} else {
						send_response(node, "%s\n", "resp,fail,reason,unauthorized");
					}
				}
				if(GETTHOR(node)->rpc.return_value.response)
					sqlite3_free(GETTHOR(node)->rpc.return_value.response);
			}
				break;
			case update: {
				int table = find_cmd(strtok(NULL, ","));
				if(table == user) {

				}
			}
				break;
			};
		}
			break;
		};
	}

}

