/*
 * thor.c
 *
 *  Created on: May 27, 2020
 *      Author: khubaibumer
 */

#include <thor.h>
#include <stdio.h>
#include <list.h>
#include <errno.h>

#pragma GCC diagnostic ignored "-Wdiscarded-qualifiers"

PRIVATE const char root[] = "/thor";
PRIVATE const char config[] = "thor.cfg";

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
extern int __create_usr_table(void *ptr, const char *uname, const char *upsswd, int role);
extern int __init_sqlite3_instance(void *ptr);
extern int __compute_hash(void *ptr, const char *key, char **hash);
extern int __get_usr_role(void *ptr, const char *name, const char *pass);
extern void __set_state (int state);
extern int __get_state ();
extern void remove_escape(char *name, size_t *inlen);
extern void __process_cmd(void *node, char *cmd);
extern void __close_client(void *ptr);
extern void __free(void *node);
extern int __log_rpc_command(void *ptr, int type, const char *process, const char *command);
extern int __get_all_users(void *ptr);
extern int __add_items(void *ptr, const char*, const char*, const char*, const char*);
extern int __get_all_items(void *ptr);
extern int __delete_item(void *ptr, const char *name);
extern char* __copy_string(char*, size_t);
extern int __update_user_info(void *ptr, const char *qual, int key, const char *updated);
extern int __update_items_info(void *ptr, const char *qual, int key, const char *updated);
extern int __send_qr_to_ui(void *ptr, FILE *ui, const char *information);
extern int __update_item_image(void *ptr, const char *qual, int key,
		const char *updated);
extern int __add_image(void *ptr, const char *upc, const char *image);
extern int __delete_image(void *ptr, const char *upc);
extern int __get_all_image_upc(void *ptr);
extern int __get_image_for_upc(void *ptr, const char *upc);
extern void set_logging_level(const char *value);
extern char* get_loggin_level(void);
extern char *get_all_log_levels();

typedef int (*rpc_command_t)(void *, int, char *, char *, char *, char *);

static thor_data_t init_data_self = {

		.exec_flags = ROOT_USR,
		.is_logged = 1,
		.load_config = __load_cfg,
		.mknod = __mknod,
		.copy_str = __copy_string,
		.set_state = __set_state,
		.get_state = __get_state,
		.trim = remove_escape,
		.free = __free,

		.logger.set_mode = set_logging_level,
		.logger.get_mode = get_loggin_level,
		.logger.get_all = get_all_log_levels,

		.thread.tid = 0,
		.thread.thread_func = serve_clients,

		.db.is_open = 0,
		.db.db_hndl = NULL,
		.db.init_db = __init_sqlite3_instance,
		.db.get_role = __get_usr_role,
		.db.psswd_db.creat_usr = __create_usr_table,
		.db.psswd_db.update_usr = __update_user_info,
		.db.psswd_db.dlt_usr = __dlt_usr_info,
		.db.psswd_db.get_all = __get_all_users,
		.db.log_cmd = (rpc_command_t) __log_rpc_command,
		.db.items.add_item = __add_items,
		.db.items.get_all = __get_all_items,
		.db.items.update_item = __update_items_info,
		.db.items.dlt_item = __delete_item,
		.db.item_images_db.update_image = __update_item_image,
		.db.item_images_db.add_image = __add_image,
		.db.item_images_db.delete_image = __delete_image,
		.db.item_images_db.get_all = __get_all_image_upc,
		.db.item_images_db.get_image = __get_image_for_upc,

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
		.rpc.return_value.response = NULL,

		.ui.to_ui =  __send_qr_to_ui,
};

static thor_data_t init_data_root = {

		.exec_flags = ROOT_USR,
		.is_logged = 1,
		.load_config = __load_cfg,
		.mknod = __mknod,
		.copy_str = __copy_string,
		.set_state = __set_state,
		.get_state = __get_state,
		.trim = remove_escape,
		.free = __free,

		.logger.set_mode = set_logging_level,
		.logger.get_mode = get_loggin_level,
		.logger.get_all = get_all_log_levels,

		.thread.tid = 0,
		.thread.thread_func = serve_clients,

		.db.is_open = 0,
		.db.db_hndl = NULL,
		.db.init_db = __init_sqlite3_instance,
		.db.get_role = __get_usr_role,
		.db.psswd_db.creat_usr = __create_usr_table,
		.db.psswd_db.update_usr = __update_user_info,
		.db.psswd_db.dlt_usr = __dlt_usr_info,
		.db.psswd_db.get_all = __get_all_users,
		.db.log_cmd = (rpc_command_t) __log_rpc_command,
		.db.items.add_item = __add_items,
		.db.items.get_all = __get_all_items,
		.db.items.update_item = __update_items_info,
		.db.items.dlt_item = __delete_item,
		.db.item_images_db.update_image = __update_item_image,
		.db.item_images_db.add_image = __add_image,
		.db.item_images_db.delete_image = __delete_image,
		.db.item_images_db.get_all = __get_all_image_upc,
		.db.item_images_db.get_image = __get_image_for_upc,

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
		.rpc.return_value.response = NULL,

		.ui.to_ui =  __send_qr_to_ui,
};

static thor_data_t init_data_dflt = {

		.exec_flags = DFL_USR,
		.is_logged = 1,
		.get_state = __get_state,
		.mknod = __mknod,
		.copy_str = __copy_string,
		.trim = remove_escape,

		.logger.set_mode = NULL,
		.logger.get_mode = NULL,

		.db.is_open = 0,
		.db.db_hndl = NULL,
		.db.get_role = __get_usr_role,
		.db.log_cmd = (rpc_command_t) __log_rpc_command,
		.db.items.get_all = __get_all_items,
		.db.item_images_db.get_image = __get_image_for_upc,
		.db.items.update_item = __update_items_info,

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
		.rpc.return_value.response = NULL,

		.ui.to_ui =  __send_qr_to_ui,
};

static thor_data_t init_data_elvt = {

		.exec_flags = ELVT_USR,
		.is_logged = 1,
		.mknod = __mknod,
		.get_state = __get_state,
		.copy_str = __copy_string,
		.trim = remove_escape,

		.logger.set_mode = NULL,
		.logger.get_mode = NULL,

		.db.is_open = 0,
		.db.db_hndl = NULL,
		.db.get_role = __get_usr_role,
		.db.psswd_db.update_usr = __update_user_info,
		.db.log_cmd = (rpc_command_t) __log_rpc_command,
		.db.items.get_all = __get_all_items,
		.db.items.add_item = __add_items,
		.db.items.update_item = __update_items_info,
		.db.items.dlt_item = __delete_item,
		.db.item_images_db.update_image = __update_item_image,
		.db.item_images_db.add_image = __add_image,
		.db.item_images_db.delete_image = __delete_image,
		.db.item_images_db.get_all = __get_all_image_upc,
		.db.item_images_db.get_image = __get_image_for_upc,
		.db.items.update_item = __update_items_info,

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
		.rpc.return_value.response = NULL,

		.ui.to_ui =  __send_qr_to_ui,
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

char* __copy_string(char *s1, size_t max_len) {

	size_t len = strnlen(s1, max_len) + 1;
	char *s2 = calloc(len, sizeof(char));
	memcpy(s2,s1,len);

	return s2;
}

int __load_cfg (void *ptr) {

	DECLARE_STATIC_SYMBOL(const char*, ipK) = "IP=";
	DECLARE_STATIC_SYMBOL(const char*, prtK) = "PORT=";
	DECLARE_STATIC_SYMBOL(const char*, crtK) = "CERT=";
	DECLARE_STATIC_SYMBOL(const char*, logK) = "LOG=";
	DECLARE_STATIC_SYMBOL(const char*, dbK) = "DB=";
	DECLARE_STATIC_SYMBOL(const char*, taxK) = "TAX=";

	unsigned char cfg[64] = {};
	sprintf(cfg, "%s/%s", root, config);

	FILE *cfp = fopen(cfg, "r");
	if(cfp == NULL) {
		// store defaults
		perror("Invalid Installation Directory");
		exit(errno);
	}

	char *ip = NULL;
	char *port = NULL;
	char *cer = NULL;
	char *logF = NULL;
	char *db = NULL;
	char *tax = NULL;
	char *tmp = NULL;
	unsigned char buf[1024] = { };

	if (fgets(&buf[0], 1023, cfp)) {
		tmp = strtok(buf, ipK);
		if (tmp)
			ip = CAST(THIS)->copy_str(tmp, 1023);
	}
	if (fgets(&buf[0], 1023, cfp)) {
		tmp = strtok(buf, prtK);
		if (tmp)
			port = CAST(THIS)->copy_str(tmp, 1023);
	}
	if (fgets(&buf[0], 1023, cfp)) {
		tmp = strtok(buf, crtK);
		if (tmp)
			cer = CAST(THIS)->copy_str(tmp, 1023);
	}
	if (fgets(&buf[0], 1023, cfp)) {
		tmp = strtok(buf, logK);
		if (tmp)
			logF = CAST(THIS)->copy_str(tmp, 1023);
	}
	if (fgets(&buf[0], 1023, cfp)) {
		tmp = strtok(buf, dbK);
		if (tmp)
			db = CAST(THIS)->copy_str(tmp, 1023);
	}
	if (fgets(&buf[0], 1023, cfp)) {
		tmp = strtok(buf, taxK);
		if (tmp)
			tax = CAST(THIS)->copy_str(tmp, 1023);
	}

	size_t len = strnlen(db, 1023)+1;
	CAST(ptr)->trim(db, &len);

	len = strnlen(port, 1023)+1;
	CAST(ptr)->trim(port, &len);

	len = strnlen(ip, 1023)+1;
	CAST(ptr)->trim(ip, &len);

	len = strnlen(cer, 1023)+1;
	CAST(ptr)->trim(cer, &len);

	len = strnlen(logF, 1023)+1;
	CAST(ptr)->trim(logF, &len);

	len = strnlen(db, 1023)+1;
	CAST(ptr)->trim(db, &len);

	len = strnlen(tax, 1023)+1;
	CAST(ptr)->trim(tax, &len);

	CAST(ptr)->db.db_name = (db);
	CAST(ptr)->server.port = atoi(port);
	CAST(ptr)->ssl_tls.certifiate = (cer);
	log.set_file(logF);
	CAST(ptr)->server.ip = (ip);
	CAST(ptr)->logfile = logF;
	CAST(ptr)->tax = tax;

	log.i("%s %s\n", ipK, ip);
	log.i("%s %s\n", prtK, port);
	log.i("%s %s\n", crtK, cer);
	log.i("%s %s\n", logK, logF);
	log.i("%s %s\n", dbK, db);

	fclose(cfp);
	return 0;
}

void __free(void *node) {

	free(GETTHOR(node)->client.ip);
}

static thor_data_t *data = &init_data_self;

void reset() {
	memcpy(data, &init_data_root, sizeof(thor_data_t));
}

void* thor_() {	return data; }
