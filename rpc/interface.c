/*
 * interface.c
 *
 *  Created on: Jun 5, 2020
 *      Author: khubaibumer
 */

#include <rpc.h>

extern int __process_db_cmds(void *node);
extern int __exec_rpc_cmd(void *node, char *command);
extern void __open_shell(void *node, char *command);

void __process_cmd(void *node, char *cmd) {

	if (strstr(cmd, "bye")) {
		// We need to close client connection respectfuly
		GETTHOR(node)->ssl_tls.close(GETTHOR(node));
		CAST(THIS)->free(node);

		return; // Done and done
	}

	/*	Other than bye are valid rpc_calls	*/
	if (strstr(cmd, "<$#EOT#$>") != NULL && strstr(cmd, ",")) {
		switch (find_cmd(strtok(cmd, ","))) {
		case auth:
			send_response(node, "auth,status,logged-in");
			break; // Nothing to do
		case db: {
			__process_db_cmds(node);
		}
			break;
		case logcmd: {
			char *process = strtok(NULL, ",");
			char *latlong = strtok(NULL, ",");
			char *tx_id = strtok(NULL, ",");
			char *command = strtok(NULL, ",");

			/* We need to repace that _ with , */
			char *tmp = strchr(latlong, '_');
			*tmp = ',';

			GETTHOR(node)->db.log_cmd(GETTHOR(node), REQD, process, command,
					latlong, tx_id); // log every command

			break;
		}
		case rpccmd: {
			char *process = strtok(NULL, ",");

			switch (find_cmd(process)) {
			case clinode: {
				log.i("Got: %s From Client: %d\n", cmd,
				GETTHOR(node)->user.uid);

				char *latlong = strtok(NULL, ",");
				char *tx_id = strtok(NULL, ",");
				char *command = strtok(NULL, "<$#EOT#$>");

				command[strlen(command) - 1] = '\0';

				/* We need to repace that _ with , */
				char *tmp = strchr(latlong, '_');
				*tmp = ',';

				if (GETTHOR(node)->is_logged) // If User is set to be logged for every rpc-cmd
					GETTHOR(node)->db.log_cmd(GETTHOR(node), AUTO, process,
							command, latlong, tx_id); // log every command

				__exec_rpc_cmd(node, command);
			}
				break;
			case superuser: {
				char *command = strtok(NULL, ",");
				__open_shell(node, command);
			}
				break;

			default:
				//error Response
				log.f("Got: %s From Client: %d\n", cmd,
				GETTHOR(node)->user.uid);
				send_response(node, "%s", "resp,fail,reason,invalid command");
				break;
			};

		}
			break;
		case control: {
			char *cmd = strtok(NULL, ",");
			switch (find_cmd(cmd)) {
			case gettax: {
				send_response(node, "resp,ok,0,response,%s", CAST(THIS)->tax);
			}
				break;
			case setlogginglevel: {
				char *log_level = strtok(NULL, ",");
				if (GETTHOR(node)->logger.set_mode) {
					CAST(THIS)->logger.set_mode(log_level);
					send_response(node, "resp,ok,0,response,%s", "success");
				} else
					send_response(node, "%s", "resp,fail,reason,un-authorized");
			}
				break;
			case getlogginglevel: {
				if (GETTHOR(node)->logger.get_mode) {
					char *mode = CAST(THIS)->logger.get_mode();
					send_response(node, "resp,ok,0,response,%s", mode);
				} else
					send_response(node, "%s", "resp,fail,reason,un-authorized");
			}
				break;
			case getloglevels: {
				if (GETTHOR(node)->logger.get_all) {
					char *_all = CAST(THIS)->logger.get_all();
					send_response(node, "resp,ok,0,response,%s", _all);
					free(_all);
				} else
					send_response(node, "%s", "resp,fail,reason,un-authorized");
			}
				break;
			case listpeers: {
				if (GETTHOR(node)->server.list) {
					GETTHOR(node)->server.list(GETTHOR(node));
					send_response(node, "resp,ok,0,response,%s",
					GETTHOR(node)->rpc.return_value.ret.value);
					free(GETTHOR(node)->rpc.return_value.ret.value);
				} else {
					send_response(node, "%s", "resp,fail,reason,un-authorized");
				}
			}
				break;
			case kickpeer: {
				send_response(node, "%s", "resp,status,under-way");
			}
				break;
			case turndown: {
				if (GETTHOR(node)->server.down) {
					send_response(node, "resp,ok,0,response,successful");
					GETTHOR(node)->server.down(GETTHOR(node));
				} else {
					send_response(node, "%s", "resp,fail,reason,un-authorized");
				}
			}
				break;
			case iteminterface: {
				char *action = strtok(NULL, ",");
				switch (find_cmd(action)) {
				case execquery: {
					send_response(node, "%s", "resp,status,under-way");
				}
					break;
				default:
					send_response(node, "%s",
							"resp,fail,reason,invalid command");
					break;
				};
			}
				break;
			};
		}
			break;

		default:
			// error Response
			send_response(node, "%s", "resp,fail,reason,invalid command");
			break;
		};
	} else {
		send_response(node, "%s", "resp,fail,reason,invalid command format");
	}
}

