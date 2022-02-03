/*
 * main.c
 *
 *  Created on: May 27, 2020
 *      Author: khubaibumer
 */

#define _GNU_SOURCE 1  /* To pick up REG_RIP */

#include <signal.h> // for sigsetjmp & siglongjmp
#include <ucontext.h> // for context

#include <stdio.h>
#include <stdlib.h>

#include <thor.h>

#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <signal.h>

extern int register_users();

extern void reset();

static void signal_handler(int sig, siginfo_t *info, void *ucontext) {

	/* Assuming our architecture is Intel x86_64. */
	ucontext_t *uc = (ucontext_t*) ucontext;
	greg_t *rip = &uc->uc_mcontext.gregs[REG_RIP];

	asm volatile (
			"movabs $checkpoint, %0"
			: "=r" (*rip)
	);

	CAST(THIS)->server.down(THIS);
	pthread_cancel(CAST(THIS)->thread.tid);
	CAST(THIS)->thread.tid = 0;
	reset();
	/* Assign a new value to *rip somehow, which will be where the
	 execution will continue after the signal handler returns. */
}

int main(int argc, char **argv) {
	/*	let's create a checkpoint to return after sigsegv	*/
	asm volatile (
			"checkpoint: .global checkpoint" :
	);

	/*	Let's create signal masks	*/
	struct sigaction sa;
	int err;

	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = signal_handler;

	err = sigemptyset(&sa.sa_mask);
	if (err)
		return 1;

	err = sigaddset(&sa.sa_mask, SIGSEGV);
	if (err)
		return 1;

	err = sigaction(SIGSEGV, &sa, NULL);
	if (err)
		return 1;

	sigset_t set;
	sigaddset(&set, SIGPIPE);
	if (sigprocmask(SIG_BLOCK, &set, NULL) == -1)
		perror("sigprocmask");

	CAST(THIS)->load_config(THIS);
	CAST(THIS)->db.init_db(THIS);
	register_users();

//	__get_all_users(THIS);

	CAST(THIS)->server.up(THIS);
	CAST(THIS)->server.accept(THIS);
	CAST(THIS)->server.list(THIS);
	CAST(THIS)->server.down(THIS);

	return 0;
}
