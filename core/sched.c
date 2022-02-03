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

void __set_state(int state) { process_state = state; }

int __get_state() {	return process_state; }

void *serve_clients(void *ptr) {
    sleep(10);
    while (1) {
        void *node = CAST(THIS)->ctrl.list_head;
        for (int i = 0; i < CAST(THIS)->ctrl.actv_client_count && process_state == STATE_RUN && node != NULL; i++) {
            errno = 0;
            if (node) {
                if (GETTHOR(node)->client.is_connected != 0) {
                    char *buf = calloc(MB(15), sizeof(char));
                    char _buf[8000] = {};
                    int bytes = 0;
                    do {
                        bytes = GETTHOR(node)->ssl_tls.read(GETTHOR(node), _buf, 7999);
                        if (bytes == -1 || bytes == 0)
                            break;
                        strcat(buf, _buf);
                    } while (strstr(_buf, "<$#EOT#$>") == NULL);

                    size_t bsz = strlen(buf);
                    GETTHOR(node)->trim(buf, &bsz);
                    if (bytes != -1 && bytes != 0) {
                        GETTHOR(node)->rpc.rpc_call(node, buf);
                    } else {
                        if (bytes == 0) {
                            GETTHOR(node)->client.is_connected = 0;
                        }
                    }
                    free(buf);
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

