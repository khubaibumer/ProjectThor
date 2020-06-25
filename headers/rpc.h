/*
 * rpc.h
 *
 *  Created on: Jun 12, 2020
 *      Author: khubaibumer
 */

#ifndef RPC_H_
#define RPC_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

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
	rpccmd,
	clinode,
	getlist,
	items,
	itemname,
	itemcount,
	itemprice,
	itemextra,
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
		{ "rpc-cmd", rpccmd },
		{ "cli-node", clinode },
		{ "get-list", getlist },
		{ "items", items },
		{ "item-name", itemname },
		{ "item-quantity", itemcount },
		{ "item-price", itemprice },
		{ "item-extra", itemextra },
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

static inline int send_response(void *node, const char *fmt, ...) {
	char *repose = calloc(5*1000*1000, sizeof(char));
	va_list arg;
	va_start(arg, fmt);
	int size = vsprintf(repose, fmt, arg);
	va_end(arg);

	int ret = GETTHOR(node)->ssl_tls.write(GETTHOR(node), repose, size);
	free(repose);

	return ret;
}

#endif /* RPC_H_ */
