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

#define DECLARE_SYMBOL(__type,  __sym) __type __sym
#define DECLARE_STATIC_SYMBOL(__type, __sym) static __type __sym
#define CAST(x) ((thor_data_t*) x)

typedef struct thor_data {

	/*	Flags of execution	*/
	uint8_t exec_flags;
	struct {
		struct {
			long alias;
			uint8_t key[64];
			uint8_t iv[32];
		} secure;

		uint32_t uid;
		uint32_t gid;
	} user; // User data

	struct {
		int port;
		char ip[16];
		struct {
			int fd;
		} sock;

		void (*up)(void);
		void (*listen)(void);
		void (*accept)(void);
		void (*list)(void);
		void (*kick)(int);
		void (*down)(void);
	} server;

	struct {
		int fd;
		int port;
		char ip[16];
	} client;

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
