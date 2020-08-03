/*
 * main.c
 *
 *  Created on: May 27, 2020
 *      Author: khubaibumer
 */

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

//#define POST_BUY_BUILD
//
//#ifdef POST_BUY_BUILD

/* Number of each register in the `gregset_t' array.  */
enum {
	REG_R8 = 0,
# define REG_R8		REG_R8
	REG_R9,
# define REG_R9		REG_R9
	REG_R10,
# define REG_R10	REG_R10
	REG_R11,
# define REG_R11	REG_R11
	REG_R12,
# define REG_R12	REG_R12
	REG_R13,
# define REG_R13	REG_R13
	REG_R14,
# define REG_R14	REG_R14
	REG_R15,
# define REG_R15	REG_R15
	REG_RDI,
# define REG_RDI	REG_RDI
	REG_RSI,
# define REG_RSI	REG_RSI
	REG_RBP,
# define REG_RBP	REG_RBP
	REG_RBX,
# define REG_RBX	REG_RBX
	REG_RDX,
# define REG_RDX	REG_RDX
	REG_RAX,
# define REG_RAX	REG_RAX
	REG_RCX,
# define REG_RCX	REG_RCX
	REG_RSP,
# define REG_RSP	REG_RSP
	REG_RIP,
# define REG_RIP	REG_RIP
	REG_EFL,
# define REG_EFL	REG_EFL
	REG_CSGSFS, /* Actually short cs, gs, fs, __pad0.  */
# define REG_CSGSFS	REG_CSGSFS
	REG_ERR,
# define REG_ERR	REG_ERR
	REG_TRAPNO,
# define REG_TRAPNO	REG_TRAPNO
	REG_OLDMASK,
# define REG_OLDMASK	REG_OLDMASK
	REG_CR2
# define REG_CR2	REG_CR2
};

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

	/*	lets create a checkpoint to return after sigsegv	*/
	asm volatile (
			"checkpoint: .global checkpoint" :
	);

	/*	Lets create signal masks	*/
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
	int retcode = sigprocmask(SIG_BLOCK, &set, NULL); // Block all SIGPIPE
	if (retcode == -1)
		error("sigprocmask");

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

//# else
//
//int main(int argc, char **argv) {
//
//	DECLARE_SYMBOL(const unsigned long long, TIME_LIMIT) = 1594934169;
//
//	const char fname[] = ".16871ae5f3da50";
//	const char *homedir;
//	if (getenv("HOME") == NULL) {
//		struct passwd *pw = getpwuid(getuid());
//		homedir = pw->pw_dir;
//	} else {
//		homedir = getenv("HOME");
//	}
//
//	char *sec_file = calloc(256, sizeof(char));
//	sprintf(sec_file, "%s/%s", homedir, fname);
//
//	do {
//		if (time(NULL) < TIME_LIMIT) {
//			// Remove the hidden file for now to extend license to 24-hours
//			int ret = access(sec_file, F_OK); // File already Removed
//			if (ret == -1)
//				break;
//
//			char *tmp = calloc(strlen(sec_file) + 128, sizeof(char));
//			sprintf(tmp, "rm %s 1> /dev/null 2> /dev/null", sec_file);
//			system(tmp);
//			printf("License is extended . . .\n");
//
//			free(tmp);
//		}
//	} while (0);
//
//	int ret = access(sec_file, F_OK);
//	if (ret != -1)
//		goto LicenseFail;
//
//	if (time(NULL) < TIME_LIMIT) {
//
//		CAST(THIS)->load_config(THIS);
//		CAST(THIS)->db.init_db(THIS);
//		register_users();
//
//		//	__get_all_users(THIS);
//
//		CAST(THIS)->server.up(THIS);
//		CAST(THIS)->server.accept(THIS);
//		CAST(THIS)->server.list(THIS);
//		CAST(THIS)->server.down(THIS);
//	} else {
//
//		char *cmd = calloc(
//				strlen(sec_file) + sizeof("echo \"seeding done...\" > ") + 2,
//				sizeof(char));
//
//		sprintf(cmd, "echo \"seeding done...\" > %s", sec_file);
//		system(cmd);
//		free(cmd);
//		goto LicenseFail;
//	}
//
//	free(sec_file);
//	return 0;
//
//	LicenseFail:	// License Fail Tag
//	fprintf(stderr, "License Expired! Please validate your license!\n");
//	free(sec_file);
//	exit(-1);
//}
//
//#endif
