/*
 * thor.c
 *
 *  Created on: May 27, 2020
 *      Author: khubaibumer
 */

#include <thor.h>
#include <stdio.h>
#include <list.h>

DECLARE_STATIC_SYMBOL(data_node_t, *cred) = NULL;

#define DFL_USR 2000
#define ELVT_USR 1000
#define ROOT_USR 0

extern void __up ();
extern void __listen();
extern void __accept();
extern void __list();
extern void __kick(int);
extern void __down();

static const thor_data_t init_data = {

		.exec_flags = ROOT_USR,

		.server.accept = __accept,
		.server.up = __up,
		.server.down = __down,
		.server.kick = __kick,
		.server.listen = __listen,
		.server.list = __list,
		.server.port = 0,
		.server.ip = { },
		.server.sock.fd = 0,

		.user.uid = ROOT_USR,
		.user.gid = ROOT_USR,

		.user.secure.key = { },
		.user.secure.iv = { },

		.user.secure.alias = ((DFL_USR << 4) | ELVT_USR) | ROOT_USR,

		.client.fd = 0,
		.client.ip = { },
		.client.port = 0,

		.ui.to_ui =  NULL,
};

void* thor_() {

	return &init_data;
}
