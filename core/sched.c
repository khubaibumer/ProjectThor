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

void* serve_clients(void *ptr) {

	sleep(10);
	while (1) {

		void *node = CAST(THIS)->ctrl.list_head;

		for (int i = 0;
				i < CAST(THIS)->ctrl.actv_client_count
						&& process_state == STATE_RUN && node != NULL; i++) {

			errno = 0;
			if (node) {

				if (GETTHOR(node)->client.is_connected != 0) {

					char buf[8000] = { };
					int bytes = GETTHOR(node)->ssl_tls.read(GETTHOR(node), buf,
							7999);

					size_t bsz = strlen(buf);
					GETTHOR(node)->trim(buf, &bsz);
					if (bytes != -1 && bytes != 0) {
						log.i("Got: %s From Client: %d\n", buf,
						GETTHOR(node)->user.uid);

						GETTHOR(node)->rpc.rpc_call(node, buf);
					} else {
						if (bytes == 0) {
							GETTHOR(node)->client.is_connected = 0;
						}
					}
				}
			}
			node = get_next_node(&node);
		}

		if (process_state == STATE_CLOSE)
			break;

		usleep(SCH_DELAY * 1000);
	}

	return 0;
}

