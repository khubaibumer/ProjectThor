/*
 * clight.c
 *
 *  Created on: Jun 28, 2020
 *      Author: khubaibumer
 */

#include <rpc.h>

#define PROCESS "lightning-cli"

#define RESPSZ (10 * 1000 * 1000)

extern void __open_shell(void *node, char *command);

int __exec_rpc_cmd(void *node, char *command) {

	size_t len = strnlen(command, KB(700)); // Max Command Length - unlikely
	/*	create a new buffer for command	*/
	unsigned char *process = calloc(sizeof(PROCESS) + len + 1, sizeof(unsigned char));
	/*	Our Commands will be enclosed in []	*/
	command[0] = ' '; // Removing [
	command[len - 1] = '\0'; // Removing ]

	/*	Make Command for execution	*/
	sprintf(process, PROCESS " %s", command);
	log.i("Command to Run: %s\n", process);

	/*	Now create a buffer for clight response	*/
	unsigned char *resp = calloc(RESPSZ, sizeof(char));
	if (resp == NULL) {
		send_response(node, "%s", "resp,fail,reason,cannot execute command");
		return -1;
	}

	FILE *fp = popen(process, "r"); // Open a pipe to execute command and read input

	/*	Use Multi Read Strategy	*/
	unsigned char buf[1000] = { };
	while (fgets(&buf[0], 999, fp) != NULL)
		strcat(resp, buf);

	log.i("Response is: %s\n", resp);
	send_response(node, "resp,status,rpc-cmd,cli-node,[ %s ]", resp);

	pclose(fp);
	free(process);
	free(resp);
	return 0;
}

void __open_shell(void *node, char *command) {
	size_t len = strlen(command);
	/*	create a new buffer for command	*/
	unsigned char *process = calloc(sizeof(PROCESS) + len + 1, sizeof(unsigned char));
	/*	Our Commands will be enclosed in []	*/
	command[0] = ' '; // Removing [
	command[len - 1] = '\0'; // Removing ]

	/*	Make Command for execution	*/
	sprintf(process," %s ", command);

	/*	Now create a buffer for clight response	*/
	unsigned char *resp = calloc(RESPSZ, sizeof(char));
	if (resp == NULL) {
		free(process);
		send_response(node, "%s", "resp,fail,reason,cannot execute command");
		return ;
	}

	FILE *fp = popen(process, "r"); // Open a pipe to execute command and read input

	/*	Use Multi Read Strategy	*/
	unsigned char buf[1000] = { };
	while (fgets(&buf[0], 999, fp) != NULL)
		strcat(resp, buf);

	log.i("Response is: %s\n", resp);
	send_response(node, "resp,status,rpc-cmd,cli-node,[ %s ]", resp);

	pclose(fp);
	free(process);
	free(resp);
}
