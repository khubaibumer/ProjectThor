/*
 * clight.c
 *
 *  Created on: Jun 28, 2020
 *      Author: khubaibumer
 */

#include <rpc.h>

#define RESPSZ (10 * 1000 * 1000)

int __exec_rpc_cmd(void *node, char *command) {

	size_t len = strlen(command);
	/*	Our Commands will be enclosed in []	*/
	command[0] = ' '; // Removing [
	command[len - 1] = '\0'; // Removing ]
	log.i("Command to Run: %s\n", command);

	/*	Now create a buffer for clight response	*/
	unsigned char *resp = calloc(RESPSZ, sizeof(char));
	if (resp == NULL) {
		send_response(node, "%s\n", "resp,fail,reason,cannot execute command");
		return -1;
	}

	FILE *fp = popen(command, "r"); // Open a pipe to execute command and read input

	/*	Use Multi Read Strategy	*/
	unsigned char buf[1000] = { };
	while (fgets(&buf[0], 999, fp) != NULL)
		strcat(resp, buf);

	log.i("Response is: %s\n", resp);
	send_response(node, "resp,status,rpc-cmd,cli-node,[ %s ]\n", resp);

	pclose(fp);
	free(resp);
	return 0;
}
