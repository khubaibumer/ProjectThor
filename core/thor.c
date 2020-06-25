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

PRIVATE const char root[] = "/thor";
PRIVATE const char config[] = "thor.config";

extern int __load_cfg (void*);
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
extern void remove_escape(char *name, size_t *inlen);
extern void __process_cmd(void *node, char *cmd);
extern void __close_client(void *ptr);
extern void __free(void *node);
extern int __log_rpc_command(void *ptr, const char *process, const char *command);
extern int __get_all_users(void *ptr);
extern int __add_items(void *ptr, const char*, const char*, const char*, const char*);
extern int __get_all_items(void *ptr);
extern int __delete_item(void *ptr, const char *name);

static const thor_data_t init_data_root = {

		.exec_flags = ROOT_USR,
		.load_config = __load_cfg,
		.mknod = __mknod,
		.set_state = __set_state,
		.trim = remove_escape,
		.free = __free,

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
		.db.psswd_db.get_all = __get_all_users,
		.db.log_cmd = __log_rpc_command,
		.db.items.add_item = __add_items,
		.db.items.get_all = __get_all_items,
		.db.items.update_item = NULL,
		.db.items.dlt_item = __delete_item,

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
		.ssl_tls.close = __close_client,
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

		.client.is_connected = 0,
		.client.fd = 0,
		.client.ip = NULL,
		.client.max_count = 32,
		.client.port = 0,

		.rpc.rpc_call = __process_cmd,

		.ui.to_ui =  NULL,
};

static const thor_data_t init_data_dflt = {

		.exec_flags = DFL_USR,
		.mknod = __mknod,
		.trim = remove_escape,

		.db.is_open = 0,
		.db.db_name = "thor.db",
		.db.db_hndl = NULL,
		.db.get_role = __get_usr_role,
		.db.log_cmd = __log_rpc_command,
		.db.items.get_all = __get_all_items,

		.use_ssl = 1,
		.ssl_tls.hash =__compute_hash,
		.ssl_tls.read = thor_reader,
		.ssl_tls.write = thor_writer,
		.ssl_tls.close = __close_client,
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

		.rpc.rpc_call = __process_cmd,

};

static const thor_data_t init_data_elvt = {

		.exec_flags = ELVT_USR,
		.mknod = __mknod,
		.trim = remove_escape,

		.db.is_open = 0,
		.db.db_name = "thor.db",
		.db.db_hndl = NULL,
		.db.get_role = __get_usr_role,
		.db.psswd_db.update_usr = __update_usr_table,
		.db.log_cmd = __log_rpc_command,
		.db.items.get_all = __get_all_items,
		.db.items.add_item = __add_items,
		.db.items.update_item = NULL,
		.db.items.dlt_item = __delete_item,

		.use_ssl = 1,
		.ssl_tls.hash =__compute_hash,
		.ssl_tls.read = thor_reader,
		.ssl_tls.write = thor_writer,
		.ssl_tls.close = __close_client,
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

		.rpc.rpc_call = __process_cmd,
};

void* __mknod(int mode) {

	static int uid = 0;
	switch(mode) {
	case DFL_USR: {
		thor_data_t *tdata = calloc(1, sizeof(thor_data_t));
		memcpy(tdata, &init_data_dflt, sizeof(thor_data_t));
		tdata->user.uid = ++uid;
		tdata->db.db_hndl = CAST(THIS)->db.db_hndl;
		return tdata;
	}
	break;
	case ELVT_USR: {
		thor_data_t *tdata = calloc(1, sizeof(thor_data_t));
		memcpy(tdata, &init_data_elvt, sizeof(thor_data_t));
		tdata->user.uid = ++uid;
		tdata->db.db_hndl = CAST(THIS)->db.db_hndl;
		return tdata;
	}
	break;
	case ROOT_USR: {
		thor_data_t *tdata = calloc(1, sizeof(thor_data_t));
		memcpy(tdata, &init_data_root, sizeof(thor_data_t));
		tdata->user.uid = ++uid;
		tdata->db.db_hndl = CAST(THIS)->db.db_hndl;
		return tdata;
	}
	break;
	};
	return NULL;
}

int __load_cfg (void *ptr) {

/*
	DECLARE_STATIC_SYMBOL(const char*, ipK) = "IP=";
	DECLARE_STATIC_SYMBOL(const char*, prtK) = "PORT=";
	DECLARE_STATIC_SYMBOL(const char*, crtK) = "CERT=";
	DECLARE_STATIC_SYMBOL(const char*, logK) = "LOG=";
	DECLARE_STATIC_SYMBOL(const char*, dbK) = "DB=";

	unsigned char cfg[64] = {};
	sprintf(cfg, "%s/%s", root, config);

	FILE *cfp = fopen(cfg, "r");
	if(cfp == NULL) {
		// store defaults
	}

	char *ip;
	char *port;
	char *cer;
	char *logF;
	char *db;
	unsigned char buf[512] = { };
	if(fgets(buf, 511, cfp)) {
		ip = strtok(buf, ipK);
		printf("%s %s\n", ipK, ip);
	}
	if(fgets(buf, 511, cfp)) {
		port = strtok(buf, prtK);
		printf("%s %s\n", prtK, port);
	}
	if(fgets(buf, 511, cfp)) {
		cer = strtok(buf, crtK);
		printf("%s %s\n", crtK, cer);
	}
	if(fgets(buf, 511, cfp)) {
		logF = strtok(buf, logK);
		printf("%s %s\n", logK, logF);
	}
	if(fgets(buf, 511, cfp)) {
		db = strtok(buf, dbK);
		printf("%s %s\n", dbK, db);
	}
*/




	return 0;
}

void __free(void *node) {

	free(GETTHOR(node)->client.ip);
}

void* thor_() {

	static thor_data_t *data = (const thor_data_t*) &init_data_root;

	return data;
}



