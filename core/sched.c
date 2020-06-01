/*
 * sched.c
 *
 *  Created on: Jun 1, 2020
 *      Author: khubaibumer
 */

#include <thor.h>

#define SCH_DELAY 500

static int process_state = STATE_INVALID;

static SSL *ssl = NULL;

void set_ssl(void *_ssl) {
	ssl = (SSL*) _ssl;
}

void __set_state(int state) {

	process_state = state;
}

/*
 void* serve_clients(void *ptr) {

 while (1) {

 void *node = CAST(THIS)->ctrl.list_head;

 for (int i = 0;
 i < CAST(THIS)->ctrl.actv_client_count
 && process_state == STATE_RUN && node != NULL; i++) {

 node = get_next_node(&CAST(THIS)->ctrl.list_head);

 if (node) {
 //				GETTHOR(node)->ssl_tls.ssl = ssl;
 int byte_flag = SSL_has_pending(ssl);
 if (byte_flag == 1) {
 char buf[1024] = { };
 int bytes = SSL_read(ssl, buf, 1023);

 //					GETTHOR(node)->ssl_tls.read(GETTHOR(node), buf, 1023);
 log.i("Got: %s From: %s\n", buf, GETTHOR(node)->client.ip);
 SSL_write(ssl, "RESP,OK,200\n", sizeof("RESP,OK,200\n"));
 }
 }
 }

 if (process_state == STATE_CLOSE)
 break;

 usleep(SCH_DELAY * 1000);

 }

 return 0;
 }
 */

void* serve_clients(void *ptr) {

	sleep(10);
	while (1) {

		void *node = CAST(THIS)->ctrl.list_head;

		for (int i = 0;
				i < CAST(THIS)->ctrl.actv_client_count
						&& process_state == STATE_RUN && node != NULL; i++) {

			node = get_next_node(&CAST(THIS)->ctrl.list_head);

			if (node) {
				GETTHOR(node)->ssl_tls.write(GETTHOR(node), "RESP,OK,200\n",
						sizeof("RESP,OK,200\n"));
				int byte_flag = SSL_has_pending(GETTHOR(node)->ssl_tls.ssl);
				if (byte_flag == 1) {
					char buf[1024] = { };
					GETTHOR(node)->ssl_tls.read(GETTHOR(node), buf, 1023);
					log.i("Got: %s From: %s\n", buf, GETTHOR(node)->client.ip);
					GETTHOR(node)->ssl_tls.write(GETTHOR(node), "RESP,OK,200\n",
							sizeof("RESP,OK,200\n"));
				}
			}
		}

		if (process_state == STATE_CLOSE)
			break;

		usleep(SCH_DELAY * 1000);

	}

	return 0;
}

