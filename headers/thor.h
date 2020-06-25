/*
 * thor.h
 *
 *  Created on: May 27, 2020
 *      Author: khubaibumer
 */

#ifndef THOR_H_
#define THOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <stdint.h>
#include <list.h>
#include <logger.h>
#include <openssl/ssl.h>
#include <sqlite3.h>
#include <pthread.h>

#define CAST(x) ((thor_data_t*) x)
#define GETTHOR(x) ((thor_data_t*) (((data_node_t*) x)->data))
#define DFL_USR 2000
#define ELVT_USR 1000
#define ROOT_USR 0

#define STATE_HELD 2
#define STATE_RUN 1
#define STATE_CLOSE 0
#define STATE_INVALID -1

#define PRIVATE __attribute__((hidden))

extern void* serve_clients(void *ptr);

typedef struct thor_data {
	/*	Flags of execution	*/
	uint32_t exec_flags;
	uint16_t use_ssl;
	int (*load_config) (void*);
	struct {
		int (*write) (void*, const void*, size_t);
		int (*read) (void*, void*, size_t);
		int (*ssl_init) (void*);
		int (*hash)(void *, const char *, char **);
		void (*close) (void*);
		SSL *ssl;
		BIO *bio;
	    SSL_CTX *ctx;
	    SSL_SESSION *session;
	} ssl_tls;

	struct {
		SSL *tssl;
		BIO *tbio;
	} tmp_cli_info;

	struct {
		struct {
			long alias;
			uint8_t key[64];
			uint8_t iv[32];

			int (*auth) (void *, int);
		} secure;

		uint32_t uid;
		uint32_t gid;
	} user; // User data

	void* (*mknod) (int);
	void (*set_state) (int);
	void (*trim) (char*, size_t*);
	void (*free) (void*);

	struct {
		int port;
		char *ip;
		struct {
			int fd;
		} sock;

		void (*up)(void*);
		void (*listen)(void*);
		void (*accept)(void*);
		void (*list)(void*);
		void (*kick)(void *, int);
		void (*down)(void*);
	} server;

	struct {
		char *db_name;
		sqlite3 *db_hndl;
		uint8_t is_open;

		int (*init_db) (void *);
		int (*get_role) (void *, const char *, const char *);
		struct {
			int (*creat_usr) (void *, const char *, const char *, int);
			int (*update_usr) (void *, const char *, const char *, const char *, const char *);
			int (*dlt_usr) (void *, const char *, const char *);
			int (*get_all) (void*);
		} psswd_db;

		struct {
			int (*add_item) (void*, const char*, const char*, const char*, const char*);
			int (*update_item) (void*, const char*, int, const char*);
			int (*dlt_item) (void*, const char*);
			int (*get_all) (void*);
		} items;

		int (*log_cmd) (void *ptr, char *proc, char *command);
	} db;

	struct {
		uint8_t is_connected;
		int fd;
		int port;
		int max_count;
		char *ip;
	} client;

	struct {
		data_node_t *list_head;
		uint16_t actv_client_count;
	} ctrl;

	struct {
		pthread_t tid;
		void* (*thread_func) (void *);
	} thread;

	struct {
		struct {
			struct {
				size_t len;
				char *value;
			} ret;
			char *response;
			int status;
		} return_value;
		void (*rpc_call) (void*, char*);
	} rpc;

	struct {
		int (*to_ui) (void*);
	} ui;
} thor_data_t;

void* thor_(void);

static inline void* thor(void) {
	return thor_();
}

#define THIS thor()

#endif /* THOR_H_ */
