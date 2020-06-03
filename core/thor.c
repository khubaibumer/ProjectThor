/*
 * thor.c
 *
 *  Created on: May 27, 2020
 *      Author: khubaibumer
 */

#include <thor.h>
#include <stdio.h>
#include <list.h>

#pragma GCC diagnostic ignored "-Wdiscarded-qualifiers"

extern void __up (void*);
extern void __listen(void*);
extern void __accept(void*);
extern void __list(void*);
extern void __kick(void*, int);
extern void __down(void*);
extern int __auth(void *,int fd);
extern void* __mknod(int mode);
extern int __ssl_initialize(void*);
extern int thor_writer(void *ptr, const void *buf, size_t len);
extern int thor_reader(void *ptr, void *buf, size_t len);
extern int __dlt_usr_info(void *ptr, const char *uname, const char *upsswd);
extern int __update_usr_table(void *ptr, const char *oname, const char *opsswd,
		const char *nname, const char *npsswd);
extern int __create_usr_table(void *ptr, const char *uname, const char *upsswd, int role);
extern int __init_sqlite3_instance(void *ptr);
extern int __compute_hash(void *ptr, const char *key, char **hash);
extern int __get_usr_role(void *ptr, const char *name, const char *pass);
extern void __set_state (int state);

static const thor_data_t init_data_root = {

		.exec_flags = ROOT_USR,
		.mknod = __mknod,
		.set_state = __set_state,
		.thread.tid = 0,
		.thread.thread_func = serve_clients,

		.db.is_open = 0,
		.db.db_hndl = NULL,
		.db.db_name = "thor.db",
		.db.init_db = __init_sqlite3_instance,
		.db.get_role = __get_usr_role,
		.db.psswd_db.creat_usr = __create_usr_table,
		.db.psswd_db.update_usr = __update_usr_table,
		.db.psswd_db.dlt_usr = __dlt_usr_info,

		.server.accept = __accept,
		.server.up = __up,
		.server.down = __down,
		.server.kick = __kick,
		.server.list = __list,
		.server.port = 0,
		.server.ip = NULL,
		.server.sock.fd = 0,

		.use_ssl = 1,
		.ssl_tls.hash =__compute_hash,
		.ssl_tls.read = thor_reader,
		.ssl_tls.write = thor_writer,
		.ssl_tls.ssl_init = __ssl_initialize,
		.ssl_tls.ssl = NULL,
		.ssl_tls.bio = NULL,
		.tmp_cli_info.tssl = NULL,
		.tmp_cli_info.tbio = NULL,

		.ctrl.actv_client_count = 0,
		.ctrl.list_head = NULL,

		.user.uid = ROOT_USR,
		.user.gid = ROOT_USR,

		.user.secure.key = { },
		.user.secure.iv = { },

		.user.secure.alias = ((DFL_USR << 4) | ELVT_USR) | ROOT_USR,
		.user.secure.auth = __auth,

		.client.fd = 0,
		.client.ip = NULL,
		.client.max_count = 32,
		.client.port = 0,

		.ui.to_ui =  NULL,
};

static const thor_data_t init_data_dflt = {

		.exec_flags = DFL_USR,
		.mknod = __mknod,

		.db.is_open = 0,
		.db.db_name = "thor.db",
		.db.db_hndl = NULL,
		.db.get_role = __get_usr_role,

		.use_ssl = 1,
		.ssl_tls.hash =__compute_hash,
		.ssl_tls.read = thor_reader,
		.ssl_tls.write = thor_writer,
		.ssl_tls.ssl_init = __ssl_initialize,
		.ssl_tls.ssl = NULL,
		.ssl_tls.bio = NULL,

		.user.uid = DFL_USR,
		.user.gid = DFL_USR,

		.user.secure.key = { },
		.user.secure.iv = { },

		.user.secure.alias = ((DFL_USR << 4) | ELVT_USR) | ROOT_USR,

		.client.fd = 0,
		.client.ip = NULL,
		.client.max_count = 32,
		.client.port = 0,
};

static const thor_data_t init_data_elvt = {

		.exec_flags = ELVT_USR,
		.mknod = __mknod,

		.db.is_open = 0,
		.db.db_name = "thor.db",
		.db.db_hndl = NULL,
		.db.get_role = __get_usr_role,
		.db.psswd_db.update_usr = __update_usr_table,

		.use_ssl = 1,
		.ssl_tls.hash =__compute_hash,
		.ssl_tls.read = thor_reader,
		.ssl_tls.write = thor_writer,
		.ssl_tls.ssl_init = __ssl_initialize,
		.ssl_tls.ssl = NULL,
		.ssl_tls.bio = NULL,

		.ctrl.actv_client_count = 0,
		.ctrl.list_head = NULL,

		.user.uid = ELVT_USR,
		.user.gid = ELVT_USR,

		.user.secure.key = { },
		.user.secure.iv = { },

		.user.secure.alias = ((DFL_USR << 4) | ELVT_USR) | ROOT_USR,

		.client.fd = 0,
		.client.ip = NULL,
		.client.max_count = 32,
		.client.port = 0,
};

void* __mknod(int mode) {

	static int uid = 0;
	switch(mode) {
	case DFL_USR: {
		thor_data_t *tdata = calloc(1, sizeof(thor_data_t));
		memcpy(tdata, &init_data_dflt, sizeof(thor_data_t));
		tdata->user.uid = ++uid;
		return tdata;
	}
	break;
	case ELVT_USR: {
		thor_data_t *tdata = calloc(1, sizeof(thor_data_t));
		memcpy(tdata, &init_data_elvt, sizeof(thor_data_t));
		tdata->user.uid = ++uid;
		return tdata;
	}
	break;
	case ROOT_USR: {
		thor_data_t *tdata = calloc(1, sizeof(thor_data_t));
		memcpy(tdata, &init_data_root, sizeof(thor_data_t));
		tdata->user.uid = ++uid;
		return tdata;
	}
	break;
	};
	return NULL;
}

void* thor_() {

	static thor_data_t data = init_data_root;

	return &data;
}



