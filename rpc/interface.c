/*
 * interface.c
 *
 *  Created on: Jun 5, 2020
 *      Author: khubaibumer
 */

#include <rpc.h>

int __process_db_cmds(void *node);

void __process_cmd(void *node, char *cmd) {

	if (strstr(cmd, "bye")) {
		// We need to close client connection respectfuly
		GETTHOR(node)->ssl_tls.close(GETTHOR(node));
		CAST(THIS)->free(node);

		return; // Done and done
	}

	/*	Other than bye are valid rpc_calls	*/
	if (strlen(cmd) > 1 && strstr(cmd, ",")) {
		switch (find_cmd(strtok(cmd, ","))) {
		case auth:
			send_response(node, "auth,status,logged-in");
			break; // Nothing to do
		case db: {
			__process_db_cmds(node);
		}
			break;
		case rpccmd: {
			char *process = strtok(NULL, ",");
			char *command = strtok(NULL, ",");
			GETTHOR(node)->db.log_cmd(GETTHOR(node), process, command); // log every command
			switch(find_cmd(process)) {
			case clinode:
				// c-lightning node command
				send_response(node, "%s\n", "resp,status,work-in-progress");
				break;

			default:
				//error Response
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
	} else {
		send_response(node, "%s\n", "resp,fail,reason,invalid command format");
	}
}

