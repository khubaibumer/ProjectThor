/*
 * sched.c
 *
 *  Created on: Jun 1, 2020
 *      Author: khubaibumer
 */

#include <thor.h>
#include <errno.h>

#define SCH_DELAY 500

static int process_state = STATE_INVALID;

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

			errno = 0;
			node = get_next_node(&CAST(THIS)->ctrl.list_head);

			if (node) {
				char buf[1024] = { };
				int bytes = GETTHOR(node)->ssl_tls.read(GETTHOR(node), buf,
						1023);
				if (bytes == -1 || bytes == 0) {
					// no data is available
					log.i("Client: %d - Read Timed Out with errno:%d\n",
							GETTHOR(node)->user.uid, errno);
					continue;
				}
				log.i("Got: %s From: %s\n", buf, GETTHOR(node)->client.ip);

				if(memcmp(buf, "BYE!\n\n", sizeof("BYE!\n\n") <= strlen(buf) ? sizeof("BYE!\n\n") : strlen(buf)) == 0) {
					// Client removed
					CAST(ptr)->ctrl.actv_client_count -= 1;
					// Add close client function to server functions
					SSL_shutdown(GETTHOR(node)->ssl_tls.ssl);
					SSL_free(GETTHOR(node)->ssl_tls.ssl);
					delete_node(&CAST(THIS)->ctrl.list_head, node);
				}

				GETTHOR(node)->ssl_tls.write(GETTHOR(node), "RESP,OK,200\n",
						sizeof("RESP,OK,200\n"));
			}
		}

		if (process_state == STATE_CLOSE)
			break;

		usleep(SCH_DELAY * 1000);
	}

	return 0;
}

